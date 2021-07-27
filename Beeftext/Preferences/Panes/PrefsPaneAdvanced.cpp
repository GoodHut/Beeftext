﻿/// \file
/// \author 
///
/// \brief Implementation of the advanced preferences pane.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "PrefsPaneAdvanced.h"
#include "Combo/ComboManager.h"
#include "SensitiveApplicationManager.h"
#include "Backup/BackupRestoreDialog.h"
#include "Backup/BackupManager.h"
#include "BeeftextGlobals.h"
#include "BeeftextUtils.h"


//**********************************************************************************************************************
///\ param[in] parent The parent widget of the pane.
//**********************************************************************************************************************
PrefsPaneAdvanced::PrefsPaneAdvanced(QWidget* parent)
   : PrefsPane(parent)
   , prefs_(PreferencesManager::instance())
{
   ui_.setupUi(this);
   ui_.spinDelayBetweenKeystrokes->setRange(PreferencesManager::minDelayBetweenKeystrokesMs(),
      PreferencesManager::maxDelayBetweenKeystrokesMs());
   if (isInPortableMode())
      ui_.frameComboListFolder->setVisible(false);

   // We update the GUI when the combo list is saved to proper enable/disable the 'Restore Backup' button
   connect(&ComboManager::instance(), &ComboManager::comboListWasSaved, this, &PrefsPaneAdvanced::updateGui);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAdvanced::load() const
{
   QSignalBlocker blocker(ui_.spinDelayBetweenKeystrokes);
   ui_.spinDelayBetweenKeystrokes->setValue(prefs_.delayBetweenKeystrokesMs());
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(prefs_.comboListFolderPath()));
   blocker = QSignalBlocker(ui_.checkWriteDebugLogFile);
   ui_.checkWriteDebugLogFile->setChecked(prefs_.writeDebugLogFile());
   blocker = QSignalBlocker(ui_.checkUseLegacyCopyPaste);
   ui_.checkUseLegacyCopyPaste->setChecked(prefs_.useLegacyCopyPaste());
   blocker = QSignalBlocker(ui_.checkUseShiftInsertForPasting);
   ui_.checkUseShiftInsertForPasting->setChecked(prefs_.useShiftInsertForPasting());
   blocker = QSignalBlocker(ui_.checkUseCustomPowershellVersion);
   ui_.checkUseCustomPowershellVersion->setChecked(prefs_.useCustomPowershellVersion());
   blocker = QSignalBlocker(ui_.editCustomPowerShellPath);
   ui_.editCustomPowerShellPath->setText(QDir::toNativeSeparators(prefs_.customPowershellPath()));
   ui_.checkAutoBackup->setChecked(prefs_.autoBackup());
   blocker = QSignalBlocker(ui_.checkUseCustomBackupLocation);
   ui_.checkUseCustomBackupLocation->setChecked(prefs_.useCustomBackupLocation());
   ui_.editCustomBackupLocation->setText(QDir::toNativeSeparators(prefs_.customBackupLocation()));

   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAdvanced::updateGui() const
{
   bool const customPowershell = ui_.checkUseCustomPowershellVersion->isChecked();
   ui_.editCustomPowerShellPath->setEnabled(customPowershell);
   ui_.buttonChangeCustomPowershellVersion->setEnabled(customPowershell);
   ui_.buttonRestoreBackup->setEnabled(BackupManager::instance().backupFileCount());
   QWidgetList widgets = { ui_.editCustomBackupLocation, ui_.buttonChangeCustomBackupLocation };
   for (QWidget* widget : widgets)
      widget->setEnabled(prefs_.useCustomBackupLocation());
}


//**********************************************************************************************************************
/// \param[in] value The new value.
//**********************************************************************************************************************
void PrefsPaneAdvanced::onSpinDelayBetweenKeystrokesChanged(int value) const
{
   prefs_.setDelayBetweenKeystrokesMs(value);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneAdvanced::onChangeComboListFolder()
{
   QString const path = QFileDialog::getExistingDirectory(this, tr("Select folder"), prefs_.comboListFolderPath());
   if (path.trimmed().isEmpty())
      return;
   if (!prefs_.setComboListFolderPath(path))
   {
      QMessageBox::critical(this, tr("Error"), tr("The location of the combo list folder could not be changed."));
      return;
   }
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneAdvanced::onResetComboListFolder()
{
   if (QMessageBox::Yes != QMessageBox::question(this, tr("Reset Folder"), tr("Reset the combo list folder?")))
      return;
   //previousComboListPath_ = prefs_.comboListFolderPath();
   QString const path = PreferencesManager::defaultComboListFolderPath();
   if (!prefs_.setComboListFolderPath(path))
   {
      QMessageBox::critical(this, tr("Error"), tr("The location of the combo list folder could not be reset."));
      return;
   }
   ui_.editComboListFolder->setText(QDir::toNativeSeparators(path));
   ComboManager::instance().saveComboListToFile();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAdvanced::onOpenComboListFolder() const
{
   QString const path = QDir::fromNativeSeparators(ui_.editComboListFolder->text());
   if (QDir(path).exists())
      QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PrefsPaneAdvanced::onCheckWriteDebugLogFile(bool checked) const
{
   prefs_.setWriteDebugLogFile(checked);
   xmilib::DebugLog& log = globals::debugLog();
   if (checked)
      log.enableLoggingToFile(globals::logFilePath());
   else
      log.disableLoggingToFile();
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PrefsPaneAdvanced::onCheckUseLegacyCopyPaste(bool checked) const
{
   prefs_.setUseLegacyCopyPaste(checked);
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked?
//**********************************************************************************************************************
void PrefsPaneAdvanced::onCheckUseShiftInsertForPasting(bool checked) const
{
   prefs_.setUseShiftInsertForPasting(checked);
}


//**********************************************************************************************************************
/// \param[in] checked Is the check box checked.
//**********************************************************************************************************************
void PrefsPaneAdvanced::onCheckUseCustomPowerShellVersion(bool checked)
{
   if (checked)
   {
      QString const path = prefs_.customPowershellPath();
      QFileInfo const fi(path);
      if (path.isEmpty() || (!fi.exists()) || (!fi.isExecutable()))
      {
         this->onChangeCustomPowershellVersion();
         if ((path.isEmpty() || (!fi.exists()) || (!fi.isExecutable())))
         {
            QSignalBlocker blocker(ui_.checkUseCustomPowershellVersion);
            ui_.checkUseCustomPowershellVersion->setChecked(false);
         }
      }
   }
   prefs_.setUseCustomPowershellVersion(ui_.checkUseCustomPowershellVersion->isChecked());
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAdvanced::onChangeCustomPowershellVersion()
{
   QString const path = QFileDialog::getOpenFileName(this, tr("Select PowerShell executable"), QString(),
      tr("Executable files (*.exe);;All files (*.*)"));
   if (path.isEmpty())
      return;
   prefs_.setCustomPowershellPath(path);
   QSignalBlocker blocker(ui_.editCustomPowerShellPath);
   ui_.editCustomPowerShellPath->setText(QDir::toNativeSeparators(path));
}


//**********************************************************************************************************************
/// \param[in] value Is the radio button checked?
//**********************************************************************************************************************
void PrefsPaneAdvanced::onCheckAutoBackup(bool value)
{
   if (!value)
   {
      if (!this->promptForAndRemoveAutoBackups()) // did the user cancel?
      {
         QSignalBlocker const blocker(ui_.checkAutoBackup);
         ui_.checkAutoBackup->setChecked(true);
      }
   }
   prefs_.setAutoBackup(value);
   this->updateGui();
}


//**********************************************************************************************************************
/// \param[in] value The check state of the box.
//**********************************************************************************************************************
void PrefsPaneAdvanced::onCheckUseCustomBackupLocation(bool value) const
{
   prefs_.setUseCustomBackupLocation(value);
   this->updateGui();
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAdvanced::onChangeCustomBackupLocation()
{
   QString const path = QFileDialog::getExistingDirectory(this, tr("Custom backup location"),
      prefs_.customBackupLocation());
   if (path.isEmpty())
      return;
   ui_.editCustomBackupLocation->setText(QDir::toNativeSeparators(path));
   prefs_.setCustomBackupLocation(path);

}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void PrefsPaneAdvanced::onRestoreBackup()
{
   BackupRestoreDialog::run(this);
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void PrefsPaneAdvanced::onEditSensitiveApplications()
{
   SensitiveApplicationManager::instance().runDialog(this);
}


//**********************************************************************************************************************
/// \return true if the used picked Yes or No
/// \return false if the user selected Cancel
//**********************************************************************************************************************
bool PrefsPaneAdvanced::promptForAndRemoveAutoBackups()
{
   BackupManager& backupManager = BackupManager::instance();
   qint32 const reply = QMessageBox::question(this, tr("Delete Backup Files?"), tr("Do you want to delete all the "
      "backup files?"), QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel),
      QMessageBox::No);
   if (QMessageBox::Cancel == reply)
      return false;
   if (QMessageBox::Yes == reply)
      backupManager.removeAllBackups();
   return true;
}
