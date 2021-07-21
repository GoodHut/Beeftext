﻿/// \file
/// \author 
///
/// \brief Implementation of emoji class.
///  
/// Copyright (c) . All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information. 


#include "stdafx.h"
#include "Emoji.h"


//**********************************************************************************************************************
/// \param[in] shortcode The shortcode for the emoji.
/// \param[in] emoji The emoji.
/// \param[in] category The category.
//**********************************************************************************************************************
Emoji::Emoji(QString const& shortcode, QString const& emoji, QString const& category)
   : shortcode_(shortcode)
   , value_(emoji)
   , category_(category)
{
}


//**********************************************************************************************************************
/// \param[in] shortcode The shortcode.
//**********************************************************************************************************************
void Emoji::setShortcode(QString const& shortcode)
{
   shortcode_ = shortcode;
}


//**********************************************************************************************************************
/// \return The shortcode.
//**********************************************************************************************************************
QString Emoji::shortcode() const
{
   return shortcode_;
}


//**********************************************************************************************************************
/// \param[in] value The value.
//**********************************************************************************************************************
void Emoji::setValue(QString const& value)
{
   value_ = value;
}


//**********************************************************************************************************************
/// \return The value.
//**********************************************************************************************************************
QString Emoji::value() const
{
   return value_;
}


//**********************************************************************************************************************
/// \param[in] category The category.
//**********************************************************************************************************************
void Emoji::setCategory(QString const& category)
{
   category_ = category;
}


//**********************************************************************************************************************
/// \return The category.
//**********************************************************************************************************************
QString Emoji::category() const
{
   return category_;
}


//**********************************************************************************************************************
/// \param[in] keywords The list of keywords
//**********************************************************************************************************************
void Emoji::setKeywords(QSet<QString> const& keywords)
{
   keywords_ = keywords;
}


//**********************************************************************************************************************
/// \return The list of keywords
//**********************************************************************************************************************
QSet<QString> const& Emoji::keywordsRef() const
{
   return keywords_;
}


//**********************************************************************************************************************
/// \return The list of keywords
//**********************************************************************************************************************
QSet<QString>& Emoji::keywordsRef()
{
   return keywords_;
}


//**********************************************************************************************************************
/// \return A string containing the emoji data.
//**********************************************************************************************************************
QString Emoji::toString() const
{
   QString result = QString("emoji: %1 - shortcode: %2 - category: %3 - keywords: { ").arg(value_).arg(shortcode_)
      .arg(category_);
   for (QString const& keyword :keywords_)
      result += keyword + " ";
   return result + "}";
}