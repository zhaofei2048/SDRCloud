
#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#endif

#include <QAudioFormat>
#include  <QImage>
#include <QPixmap>
#include <QProgressBar>
#include <QToolBar>
#include "imageviewer.h"
#include "wavfile.h"
#include "demod_core.h"
#include "aptformat.h"
#include "imageprocess.h"


//! [0]
ImageViewer::ImageViewer()
	: imageLabel(new QLabel)
	, scrollArea(new QScrollArea)
	, scaleFactor(1)
{
	QIcon icon(":/icon/Resources/sdrcloud.ico");
	setWindowIcon(icon);

	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	scrollArea->setVisible(false);
	setCentralWidget(scrollArea);

	progress = new QProgressBar();
	progress->setRange(0, 7);
	statusBar()->insertPermanentWidget(0, progress);
	/*statusBar()->insertPermanentWidget(1, statusLabel);*/

	

	createActions();

	resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

//! [0]
//! [2]

bool ImageViewer::loadFile(const QString &fileName)
{
	QImageReader reader(fileName);
	reader.setAutoTransform(true);
	const QImage newImage = reader.read();
	if (newImage.isNull()) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
			tr("Cannot load %1: %2")
			.arg(QDir::toNativeSeparators(fileName), reader.errorString()));
		return false;
	}
	//! [2]

	setImage(newImage);

	setWindowFilePath(fileName);

	const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
		.arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
	statusBar()->showMessage(message);
	return true;
}

void ImageViewer::updateStatusInfo(QString info, int p)
{
	statusBar()->showMessage(info);
	if (p > 0) {
		progress->setValue(p);
	}
}

void ImageViewer::setImage(const QImage &newImage)
{
	image = newImage;
	imageLabel->setPixmap(QPixmap::fromImage(image));
	//! [4]
	scaleFactor = 1.0;

	scrollArea->setVisible(true);
	//printAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	updateActions();

	if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();
}

//! [4]

bool ImageViewer::saveFile(const QString &fileName)
{
	QImageWriter writer(fileName);

	if (!writer.write(image)) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
			tr("Cannot write %1: %2")
			.arg(QDir::toNativeSeparators(fileName)), writer.errorString());
		return false;
	}
	const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
	statusBar()->showMessage(message);
	return true;
}

//! [1]

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
	static bool firstDialog = true;

	if (firstDialog) {
		firstDialog = false;
		const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
		dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
	}

	QStringList mimeTypeFilters;
	const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
		? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
	foreach(const QByteArray &mimeTypeName, supportedMimeTypes)
		mimeTypeFilters.append(mimeTypeName);
	mimeTypeFilters.sort();
	dialog.setMimeTypeFilters(mimeTypeFilters);
	dialog.selectMimeTypeFilter("image/jpeg");
	if (acceptMode == QFileDialog::AcceptSave)
		dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open()
{
	QFileDialog dialog(this, tr("Open File"));
	initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

	while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}
//! [1]

void ImageViewer::saveAs()
{
	QFileDialog dialog(this, tr("保存文件为"));
	initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

	while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

//! [5]
//void ImageViewer::print()
////! [5] //! [6]
//{
//	Q_ASSERT(imageLabel->pixmap());
//#if QT_CONFIG(printdialog)
//	//! [6] //! [7]
//	QPrintDialog dialog(&printer, this);
//	//! [7] //! [8]
//	if (dialog.exec()) {
//		QPainter painter(&printer);
//		QRect rect = painter.viewport();
//		QSize size = imageLabel->pixmap()->size();
//		size.scale(rect.size(), Qt::KeepAspectRatio);
//		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//		painter.setWindow(imageLabel->pixmap()->rect());
//		painter.drawPixmap(0, 0, *imageLabel->pixmap());
//	}
//#endif
//}
//! [8]

void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
	QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
	if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
		if (mimeData->hasImage()) {
			const QImage image = qvariant_cast<QImage>(mimeData->imageData());
			if (!image.isNull())
				return image;
		}
	}
	return QImage();
}
#endif // !QT_NO_CLIPBOARD

void ImageViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
	const QImage newImage = clipboardImage();
	if (newImage.isNull()) {
		statusBar()->showMessage(tr("No image in clipboard"));
	}
	else {
		setImage(newImage);
		setWindowFilePath(QString());
		const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
			.arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
		statusBar()->showMessage(message);
	}
#endif // !QT_NO_CLIPBOARD
}

//! [9]
void ImageViewer::zoomIn()
//! [9] //! [10]
{
	scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
	scaleImage(0.8);
}

//! [10] //! [11]
void ImageViewer::normalSize()
//! [11] //! [12]
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}
//! [12]

//! [13]
void ImageViewer::fitToWindow()
//! [13] //! [14]
{
	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow)
		normalSize();
	updateActions();
}
//! [14]


//! [15]
void ImageViewer::about()
//! [15] //! [16]
{
	QMessageBox::about(this, tr("关于SDRCloud"),
		tr("国科大电子系统设计课程NOAA气象卫星云图接收解码软件"));
}
void ImageViewer::importWave()
{
	// do something to import the wave
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("导入wav文件"), ".", tr("原始APT数据 (*.wav)"));
	if (fileName.isEmpty()) {
		qDebug() << "null";
	}
	else {
		qDebug() << fileName;
		wavFileName = fileName;
		statusBar()->showMessage(fileName);
		decodeAct->setEnabled(true);

		

		//WavFile wav;
		//quint64 len;
		//int sampleSize;
		//int Fs;
		//qint64 len2;
		//QAudioFormat format;

		//wav.getWaveDataSize(fileName, len, sampleSize);	// len / sampleSize = 样本点数
		//qreal *data = (qreal*)(new char[ceil(len/(sampleSize/8))*sizeof(qreal)]);

		//wav.readWave(fileName, data, len2, format);
		//qDebug() << "wav datasize = " << QString::number(len2);
		//Fs = format.sampleRate();

		//quint32 aptDataLen = 0;
		//demodAM(Fs, data, len2, data, aptDataLen);
		//qDebug() << "apt data len=" << aptDataLen;

		//// 整理为APT图像
		//unsigned int imgSize = 0;
		//unsigned int width = 0;
		//unsigned int height = 0;
		//uchar *imgarr = nullptr;
		//aptToImage(Fs, data, aptDataLen, imgarr, imgSize, width);
		//qDebug() << "imgSize=" << imgSize;
		//height = imgSize / width;
		//
		//qDebug() << "Fs=" << Fs;
		//unsigned int maxwidth = width > height ? width : height;
		//QImage img(width, height, QImage::Format_Grayscale8);
		//for (int i = 0; i < 10; i++) {
		//	qDebug() << data[i];
		//}

		//RGB分量值
		//int b = 0;
		//int g = 0;
		//int r = 0;

		////设置像素
		//for (int i = 0; i < height; i++)
		//{
		//	for (int j = 0; j < width; j++)
		//	{
		//		b = (int)imgarr[i * width + j];
		//		g = b;
		//		r = g;
		//		img.setPixel(j, i, qRgb(r, g, b));
		//	}
		//}

		///*imageLabel->setPixmap(QPixmap::fromImage(img));*/
		//setImage(img);
		//qDebug() << "img is displaying: height=" << height;
	}
}
void ImageViewer::startDecode(bool)
{
	unsigned char *imgarr = nullptr;
	int imgSize = 0;
	int width;
	int height;

	bool isSync = syncAct->isChecked();
	aptDecode(this, wavFileName, imgarr, imgSize, width, height, isSync);
	updateStatusInfo("图像显示中", 6);
	arrToImage(imgarr, width, height, image);
	setImage(image);
	updateStatusInfo("就绪", 7);
	histeqAct->setEnabled(true);
	qDebug() << "img is displaying: height=" << height;
}
void ImageViewer::doHisteq(bool)
{
	updateStatusInfo("对图像直方图均衡化中", 0);
	histeq(image);
	updateStatusInfo("就绪", 7);
	setImage(image);
}
//! [16]

//! [17]
void ImageViewer::createActions()
//! [17] //! [18]
{
	QMenu *fileMenu = menuBar()->addMenu(tr("文件"));

	QAction *openAct = fileMenu->addAction(tr("&打开图片..."), this, &ImageViewer::open);
	openAct->setShortcut(QKeySequence::Open);

	importWaveAct = fileMenu->addAction(tr("导入wav文件..."), this, &ImageViewer::importWave);

	saveAsAct = fileMenu->addAction(tr("&保存为..."), this, &ImageViewer::saveAs);
	saveAsAct->setEnabled(false);

	//printAct = fileMenu->addAction(tr("&Print..."), this, &ImageViewer::print);
	//printAct->setShortcut(QKeySequence::Print);
	//printAct->setEnabled(false);

	fileMenu->addSeparator();

	QAction *exitAct = fileMenu->addAction(tr("关闭图像窗口"), this, &QWidget::close);
	exitAct->setShortcut(tr("Ctrl+Q"));

	QMenu *editMenu = menuBar()->addMenu(tr("操作"));

	copyAct = editMenu->addAction(tr("复制"), this, &ImageViewer::copy);
	copyAct->setShortcut(QKeySequence::Copy);
	copyAct->setEnabled(false);

	QAction *pasteAct = editMenu->addAction(tr("粘贴"), this, &ImageViewer::paste);
	pasteAct->setShortcut(QKeySequence::Paste);

	QMenu *viewMenu = menuBar()->addMenu(tr("视图"));

	zoomInAct = viewMenu->addAction(tr("放大 (25%)"), this, &ImageViewer::zoomIn);
	zoomInAct->setShortcut(QKeySequence::ZoomIn);
	zoomInAct->setEnabled(false);

	zoomOutAct = viewMenu->addAction(tr("缩小 (25%)"), this, &ImageViewer::zoomOut);
	zoomOutAct->setShortcut(QKeySequence::ZoomOut);
	zoomOutAct->setEnabled(false);

	normalSizeAct = viewMenu->addAction(tr("正常大小"), this, &ImageViewer::normalSize);
	normalSizeAct->setShortcut(tr("Ctrl+S"));
	normalSizeAct->setEnabled(false);

	viewMenu->addSeparator();

	fitToWindowAct = viewMenu->addAction(tr("适应窗口"), this, &ImageViewer::fitToWindow);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));

	QMenu *helpMenu = menuBar()->addMenu(tr("帮助"));

	helpMenu->addAction(tr("关于"), this, &ImageViewer::about);
	/*helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);*/

	// 构建工具栏
	mainToolBar = addToolBar(tr("工具"));
	syncAct = mainToolBar->addAction(tr("开启同步检测"));
	decodeAct = mainToolBar->addAction(tr("解码图像"));
	histeqAct = mainToolBar->addAction(tr("直方图均衡"));
	connect(decodeAct, SIGNAL(triggered(bool)), this, SLOT(startDecode(bool)));
	connect(histeqAct, SIGNAL(triggered(bool)), this, SLOT(doHisteq(bool)));
	syncAct->setCheckable(true);
	syncAct->setChecked(false);
	decodeAct->setEnabled(false);
	histeqAct->setEnabled(false);
}
//! [18]

//! [21]
void ImageViewer::updateActions()
//! [21] //! [22]
{
	saveAsAct->setEnabled(!image.isNull());
	copyAct->setEnabled(!image.isNull());
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
	Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
	scrollBar->setValue(int(factor * scrollBar->value()
		+ ((factor - 1) * scrollBar->pageStep() / 2)));
}
//! [26]
