/*  This file is part of YUView - The YUV player with advanced analytics toolset
 *   <https://github.com/IENT/YUView>
 *   Copyright (C) 2015  Institut für Nachrichtentechnik, RWTH Aachen University, GERMANY
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   In addition, as a special exception, the copyright holders give
 *   permission to link the code of portions of this program with the
 *   OpenSSL library under certain conditions as described in each
 *   individual source file, and distribute linked combinations including
 *   the two.
 *
 *   You must obey the GNU General Public License in all respects for all
 *   of the code used other than OpenSSL. If you modify file(s), you may
 *   extend this exception to your version of the file(s), but you are not
 *   obligated to do so. If you do not wish to do so, delete this exception
 *   statement from your version of the file(s).
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "PixelZoomerWidget.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace
{

// Width of the row/col input fields (fits 5 digits).
constexpr auto PIXELZOOMER_INPUT_WIDTH = 60;
constexpr auto PIXELZOOMER_MAX_DIGITS  = 5;

} // namespace

PixelZoomerWidget::PixelZoomerWidget(QWidget *parent) : QWidget(parent)
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(4, 4, 4, 4);

  auto createLineEdit = [&]() {
    auto lineEdit = new QLineEdit(this);
    lineEdit->setValidator(new QIntValidator(0, 99999, lineEdit));
    lineEdit->setMaxLength(PIXELZOOMER_MAX_DIGITS);
    lineEdit->setPlaceholderText("0");
    lineEdit->setFixedWidth(PIXELZOOMER_INPUT_WIDTH);
    layout->addWidget(lineEdit);
    connect(lineEdit, &QLineEdit::returnPressed, this, &PixelZoomerWidget::returnPressed);
    return lineEdit;
  };

  layout->addWidget(new QLabel("Row:"));
  rowLineEdit = createLineEdit();
  layout->addWidget(new QLabel("Col:"));
  colLineEdit = createLineEdit();

  auto goButton   = new QPushButton("Go", this);
  auto backButton = new QPushButton("Back", this);
  layout->addWidget(goButton);
  layout->addWidget(backButton);

  connect(goButton, &QPushButton::clicked, this, &PixelZoomerWidget::returnPressed);
  connect(backButton, &QPushButton::clicked, this, &PixelZoomerWidget::goBackRequested);
}

void PixelZoomerWidget::returnPressed()
{
  bool okRow = false;
  bool okCol = false;
  int  row   = rowLineEdit->text().toInt(&okRow);
  int  col   = colLineEdit->text().toInt(&okCol);

  // Both fields are optional, empty input is treated as 0.
  if (!okRow && !rowLineEdit->text().isEmpty())
  {
    rowLineEdit->setFocus();
    return;
  }
  if (!okCol && !colLineEdit->text().isEmpty())
  {
    colLineEdit->setFocus();
    return;
  }

  emit pixelZoomRequested(col, row);
}
