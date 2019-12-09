#include "GraphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay(QWidget* parent)
	: QGraphicsView(parent)
{
	contextMenu.get()->addAction(actionFileNew.get());
	contextMenu.get()->addAction(actionFileOpen.get());
	contextMenu.get()->addAction(actionFileSave.get());
	contextMenu.get()->addAction(actionFileExport.get());

	this->setScene(scene.get());
	scene.get()->setParent(this->parent());

	this->setStyleSheet("border: none; background-color: #FFFFFF");
	this->setLayout(layout.get());

	layout.get()->setMargin(50);

	layout.get()->addWidget(characterSkinBtnPicker.get(), 0, 0, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterEyeBtnPicker.get(), 1, 0, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterLipBtnPicker.get(), 2, 0, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterBlushBtnPicker.get(), 3, 0, Qt::AlignLeft | Qt::AlignTop);

	layout.get()->addWidget(characterHairBtnPicker.get(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterChestBtnPicker.get(), 1, 1, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterBottomBtnPicker.get(), 2, 1, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterFeetBtnPicker.get(), 3, 1, Qt::AlignLeft | Qt::AlignTop);

	layout.get()->addWidget(characterBtnSpacerPicker1.get(), 0, 2, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterBtnSpacerPicker2.get(), 1, 2, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterBtnSpacerPicker3.get(), 2, 2, Qt::AlignLeft | Qt::AlignTop);
	layout.get()->addWidget(characterBtnSpacerPicker4.get(), 3, 2, Qt::AlignLeft | Qt::AlignTop);

	layout.get()->addWidget(characterHairBtnLeft.get(), 0, 3, Qt::AlignRight);
	layout.get()->addWidget(characterHeadBtnLeft.get(), 1, 3, Qt::AlignRight);
	layout.get()->addWidget(characterChestBtnLeft.get(), 2, 3, Qt::AlignRight);
	layout.get()->addWidget(characterBottomBtnLeft.get(), 3, 3, Qt::AlignRight);
	layout.get()->addWidget(characterFeetBtnLeft.get(), 4, 3, Qt::AlignRight);

	layout.get()->addWidget(characterHairBtnRight.get(), 0, 4, Qt::AlignRight);
	layout.get()->addWidget(characterHeadBtnRight.get(), 1, 4, Qt::AlignRight);
	layout.get()->addWidget(characterChestBtnRight.get(), 2, 4, Qt::AlignRight);
	layout.get()->addWidget(characterBottomBtnRight.get(), 3, 4, Qt::AlignRight);
	layout.get()->addWidget(characterFeetBtnRight.get(), 4, 4, Qt::AlignRight);

	// Skin color should be added first, so that it will be placed under the other elements.
	scene.get()->addItem(characterSkinColor.get());

	scene.get()->addItem(characterEyeColor.get());
	scene.get()->addItem(characterLipColor.get());
	scene.get()->addItem(characterBlushColor.get());

	scene.get()->addItem(characterHead.get());
	scene.get()->addItem(characterChest.get());
	scene.get()->addItem(characterBottom.get());
	scene.get()->addItem(characterFeet.get());

	// Hair color should be added last, so that it will be placed over the other elements.
	// (otherwise, it could be hidden under clothing, etc., in an awkward way)
	scene.get()->addItem(characterHair.get());

	connect(characterHeadBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterHead.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterHeadBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterHead.get()->moveRightInDisplay();
		characterModified = true;
	});
	connect(characterChestBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterChest.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterChestBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterChest.get()->moveRightInDisplay();
		characterModified = true;
	});
	connect(characterBottomBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterBottom.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterBottomBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterBottom.get()->moveRightInDisplay();
		characterModified = true;
	});
	connect(characterFeetBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterFeet.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterFeetBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterFeet.get()->moveRightInDisplay();
		characterModified = true;
	});
	connect(characterHairBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterHair.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterHairBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterHair.get()->moveRightInDisplay();
		characterModified = true;
	});

	connect(actionFileNew.get(), &QAction::triggered, this, [=]() {
		if (fileSaveModifCheck())
			fileNew();
	});
	connect(actionFileOpen.get(), &QAction::triggered, this, [=]() {
		if (fileSaveModifCheck())
			fileOpen();
	});
	connect(actionFileSave.get(), &QAction::triggered, this, [=]() {
		if (fileSave())
			characterModified = false;
	});
	connect(actionFileExport.get(), &QAction::triggered, this, &GraphicsDisplay::fileExportCombination);

	// temporary shortcut for testing color change in image(s)
	/*connect(shortcutChangeColor.get(), &QShortcut::activated, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterSkinColor.get()->getCurrentColor(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
			characterSkinColor.get()->setColorToScene(colorNew);
	});*/
	connect(characterSkinBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterSkinColor.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterSkinColor.get()->setColorToScene(colorNew, ColorSet::MULTIPLY);
			characterModified = true;
		}
	});
	connect(characterEyeBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterEyeColor.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterEyeColor.get()->setColorToScene(colorNew, ColorSet::FILL);
			characterModified = true;
		}
	});
	connect(characterLipBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterLipColor.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterLipColor.get()->setColorToScene(colorNew, ColorSet::FILL);
			characterModified = true;
		}
	});
	connect(characterBlushBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterBlushColor.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterBlushColor.get()->setColorToScene(colorNew, ColorSet::FILL);
			characterModified = true;
		}
	});

	connect(characterChestBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterChest.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterChest.get()->setColorToScene(colorNew, ColorSet::MULTIPLY);
			characterModified = true;
		}
	});
	connect(characterBottomBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterBottom.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterBottom.get()->setColorToScene(colorNew, ColorSet::MULTIPLY);
			characterModified = true;
		}
	});
	connect(characterFeetBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterFeet.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterFeet.get()->setColorToScene(colorNew, ColorSet::MULTIPLY);
			characterModified = true;
		}
	});
	connect(characterHairBtnPicker.get(), &QPushButton::clicked, this, [=]() {
		QColor colorNew = QColorDialog::getColor(characterHair.get()->getColorOfDisplayed(), this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			characterHair.get()->setColorToScene(colorNew, ColorSet::MULTIPLY);
			characterModified = true;
		}
	});
}

// public:

bool GraphicsDisplay::fileSaveModifCheck()
{
	if (!characterModified)
		return true;

	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this->parentWidget(), tr("Character Creator 2D"),
			tr("The character has been modified.\nDo you want to save your character changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret)
	{
	case QMessageBox::Save:
		if (fileSave())
			return true;
		else
			return false;
	case QMessageBox::Discard:
		return true;
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}

	return true;
}

// protected:

void GraphicsDisplay::contextMenuEvent(QContextMenuEvent *event)
{
	contextMenu.get()->exec(event->globalPos());
}

// private:

QString GraphicsDisplay::extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive) + 1 - strEnd.length();
			extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
			posFound = posEnd;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive) + 1 - strEnd.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	return extracted;
}

void GraphicsDisplay::fileNew()
{
	characterSkinColor.get()->setDisplayedToDefault(ColorSet::MULTIPLY);
	characterEyeColor.get()->setDisplayedToDefault(ColorSet::FILL);
	characterLipColor.get()->setDisplayedToDefault(ColorSet::FILL);
	characterBlushColor.get()->setDisplayedToDefault(ColorSet::FILL);
	characterHead.get()->setDisplayedToDefault(ColorSet::NONE);
	characterChest.get()->setDisplayedToDefault(ColorSet::MULTIPLY);
	characterBottom.get()->setDisplayedToDefault(ColorSet::MULTIPLY);
	characterFeet.get()->setDisplayedToDefault(ColorSet::MULTIPLY);
	characterHair.get()->setDisplayedToDefault(ColorSet::MULTIPLY);
	characterModified = false;
}

void GraphicsDisplay::fileOpen()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open"), fileDirLastOpened, tr("Text Files (*.txt)"));
	if (!filename.isEmpty())
	{
		QString fileContents;
		QFile fileRead(filename);
		if (fileRead.open(QIODevice::ReadOnly))
		{
			bool partsMissing = false;
			QTextStream qStream(&fileRead);
			while (!qStream.atEnd())
			{
				QString line = qStream.readLine();
				if (line.contains(characterSkinColor.get()->getPartTypeAssetStr() + "="))
				{
					if (characterSkinColor.get()->setFilenameAssetAsDisplayed(extractSubstringInbetweenQt("=", ",", line)))
					{
						characterSkinColor.get()->setColorToScene(QColor(extractSubstringInbetweenQt(",", "", line)), ColorSet::MULTIPLY);
					}
					else
						partsMissing = true;
				}
				else if (line.contains(characterEyeColor.get()->getPartTypeAssetStr() + "="))
				{
					characterEyeColor.get()->setColorToScene(QColor(extractSubstringInbetweenQt("=", "", line)), ColorSet::FILL);
				}
				else if (line.contains(characterLipColor.get()->getPartTypeAssetStr() + "="))
				{
					characterLipColor.get()->setColorToScene(QColor(extractSubstringInbetweenQt("=", "", line)), ColorSet::FILL);
				}
				else if (line.contains(characterBlushColor.get()->getPartTypeAssetStr() + "="))
				{
					characterBlushColor.get()->setColorToScene(QColor(extractSubstringInbetweenQt("=", "", line)), ColorSet::FILL);
				}
				else if (line.contains(characterHead.get()->getPartTypeAssetStr() + "="))
				{
					if (characterHead.get()->setFilenameAssetAsDisplayed(extractSubstringInbetweenQt("=", ",", line)))
					{
						characterHead.get()->setColorToScene(QColor(extractSubstringInbetweenQt(",", "", line)), ColorSet::NONE);
					}
					else
						partsMissing = true;
				}
				else if (line.contains(characterChest.get()->getPartTypeAssetStr() + "="))
				{
					if (characterChest.get()->setFilenameAssetAsDisplayed(extractSubstringInbetweenQt("=", ",", line)))
					{
						characterChest.get()->setColorToScene(QColor(extractSubstringInbetweenQt(",", "", line)), ColorSet::MULTIPLY);
					}
					else
						partsMissing = true;
				}
				else if (line.contains(characterBottom.get()->getPartTypeAssetStr() + "="))
				{
					if (characterBottom.get()->setFilenameAssetAsDisplayed(extractSubstringInbetweenQt("=", ",", line)))
					{
						characterBottom.get()->setColorToScene(QColor(extractSubstringInbetweenQt(",", "", line)), ColorSet::MULTIPLY);
					}
					else
						partsMissing = true;
				}
				else if (line.contains(characterFeet.get()->getPartTypeAssetStr() + "="))
				{
					if (characterFeet.get()->setFilenameAssetAsDisplayed(extractSubstringInbetweenQt("=", ",", line)))
					{
						characterFeet.get()->setColorToScene(QColor(extractSubstringInbetweenQt(",", "", line)), ColorSet::MULTIPLY);
					}
					else
						partsMissing = true;
				}
				else if (line.contains(characterHair.get()->getPartTypeAssetStr() + "="))
				{
					if (characterHair.get()->setFilenameAssetAsDisplayed(extractSubstringInbetweenQt("=", ",", line)))
					{
						characterHair.get()->setColorToScene(QColor(extractSubstringInbetweenQt(",", "", line)), ColorSet::MULTIPLY);
					}
					else
						partsMissing = true;
				}
			}
			fileRead.close();
			fileDirLastOpened = QFileInfo(filename).path();
			characterModified = false;
			if (partsMissing)
				QMessageBox::information(this->parentWidget(), tr("Parts Missing"), tr("One or more parts was not found when trying to load from file reference.\r\nSave file only saves references to assets, so if they are moved or deleted, loading may fail."));
		}
	}
}

bool GraphicsDisplay::fileSave()
{
	QFileDialog dialog(this, tr("Save As"), fileDirLastSaved, tr("Text Files (*.txt)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString fpath = dialog.selectedFiles().first();
		QFile fileWrite(fpath);
		if (fileWrite.open(QIODevice::WriteOnly))
		{
			QTextStream qStream(&fileWrite);

			qStream << characterSkinColor.get()->getPartTypeAssetStr() +
				"=" + characterSkinColor.get()->getFilenameOfDisplayed() +
				"," + characterSkinColor.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterEyeColor.get()->getPartTypeAssetStr() +
				"=" + characterEyeColor.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterLipColor.get()->getPartTypeAssetStr() +
				"=" + characterLipColor.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterBlushColor.get()->getPartTypeAssetStr() +
				"=" + characterBlushColor.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterHead.get()->getPartTypeAssetStr() +
				"=" + characterHead.get()->getFilenameOfDisplayed() +
				"," + characterHead.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterChest.get()->getPartTypeAssetStr() +
				"=" + characterChest.get()->getFilenameOfDisplayed() +
				"," + characterChest.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterBottom.get()->getPartTypeAssetStr() + 
				"=" + characterBottom.get()->getFilenameOfDisplayed() +
				"," + characterBottom.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterFeet.get()->getPartTypeAssetStr() + 
				"=" + characterFeet.get()->getFilenameOfDisplayed() +
				"," + characterFeet.get()->getColorOfDisplayed().name() + "\r\n";

			qStream << characterHair.get()->getPartTypeAssetStr() +
				"=" + characterHair.get()->getFilenameOfDisplayed() +
				"," + characterHair.get()->getColorOfDisplayed().name() + "\r\n";

			fileWrite.close();
			fileDirLastSaved = QFileInfo(fpath).path();
			return true;
		}
	}
	return false;
}

void GraphicsDisplay::fileExportCombination()
{
	QFileDialog dialog(this, tr("Save As"), fileDirLastExported, tr("Image Files (*.png)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString selectedFile = dialog.selectedFiles().first();
		QFile fileWrite(selectedFile);
		fileWrite.open(QIODevice::WriteOnly);
		QImage composite(scene.get()->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
		composite.fill(Qt::white);
		QPainter painter(&composite);
		scene->render(&painter);
		composite.save(&fileWrite, "PNG");
		fileDirLastExported = QFileInfo(selectedFile).path();
	}
}