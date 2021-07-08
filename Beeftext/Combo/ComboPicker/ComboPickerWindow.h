﻿/// \file
/// \author 
///
/// \brief Declaration of combo picker window
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#ifndef BEEFTEXT_COMBO_PICKER_WINDOW_H
#define BEEFTEXT_COMBO_PICKER_WINDOW_H


#include "ui_ComboPickerWindow.h"
#include "ComboPickerModel.h"
#include "ComboPickerWindowResizer.h"
#include "ComboPickerSortFilterProxyModel.h"
#include "../Combo.h"


void showComboPickerWindow(); ///< Show the combo picker window.


//**********************************************************************************************************************
/// \brief The combo picker window class
//**********************************************************************************************************************
class ComboPickerWindow: public QWidget
{
   Q_OBJECT
public: // member functions
   ComboPickerWindow(); ///< Default constructor
   ComboPickerWindow(ComboPickerWindow const&) = delete; ///< Disabled copy-constructor
   ComboPickerWindow(ComboPickerWindow&&) = delete; ///< Disabled assignment copy-constructor
   ~ComboPickerWindow() override = default; ///< Destructor
   ComboPickerWindow& operator=(ComboPickerWindow const&) = delete; ///< Disabled assignment operator
   ComboPickerWindow& operator=(ComboPickerWindow&&) = delete; ///< Disabled move assignment operator

protected:
   void keyPressEvent(QKeyEvent* event) override; ///< Key press event handler.
   void changeEvent(QEvent* event) override; ///< Change event handler.
   void showEvent(QShowEvent* event) override; ///< Show event handler.
   void mousePressEvent(QMouseEvent* event) override; ///< Mouse press event handler.
   void mouseReleaseEvent(QMouseEvent* event) override; ///< Mouse release event handler.
   bool eventFilter(QObject* watched, QEvent* event) override; ///< Event filter handler.

private slots:
   void onSearchTextChanged(QString const& text); ///< Slot for the change of the search text.
   void onItemClicked(QModelIndex const&); ///< Slot for clicking on an item.

private: // member functions
   void selectPreviousCombo() const; ///< Select the previous combo in the list.
   void selectNextCombo() const; ///< Select the next combo in the list.
   qint32 selectedComboIndex() const; ///< Retrieve the index of the selected combo.
   SpCombo selectedCombo() const; ///< Retrieve the selected combo.
   void selectComboAtIndex(qint32 index) const; ///< Select the combo at a given index
   void triggerSelectedCombo(); ///< Trigger the selected combo.

private: // data member
   Ui::ComboPickerWindow ui_ = {}; ///< The GUI for the window.
   ComboPickerModel model_; ///< The model for the list view.
   ComboPickerSortFilterProxyModel proxyModel_; ///< The proxy model for sorting/filtering the list view
   ComboPickerWindowResizer resizer_; ///< The resizer for the window.
};


#endif // #ifndef BEEFTEXT_COMBO_PICKER_WINDOW_H
