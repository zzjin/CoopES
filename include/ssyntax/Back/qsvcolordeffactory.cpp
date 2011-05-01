/**
 * \file qsvcolordeffactory.cpp
 * \brief Implementation of the color defintion factory
 * \author Diego Iastrubni (elcuco@kde.org)
 * \date 2006-07-22 01:58:07
 * License LGPL
 * \see qmdiActionGroup
 */

#include <QtDebug>
#include <QFile>

#include <QBrush>
#include <QTextCharFormat>
#include <QColor>

#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "debug_info.h"

/**
 * \class QsvColorDefFactory
 * \brief An abstract factory for languages definitions
 *
 * A factory for color definitinos. You can load this class with a file
 * and then query for a specific color role, and then get the color
 * definition 
 *
 * \see QsvColorDef
 */


/**
 * \brief default constructor
 * 
 * Builds the factory, without any color definitions. You can
 * load colors by calling load() later on.
 * 
 */
QsvColorDefFactory::QsvColorDefFactory()
{
}

/**
 * \brief constructs a color factory with a document
 * \param doc the document from which the color definitions should be loaded
 * 
 * Builds the factory, without and loading the definitions from the
 * DOM document passed as the parameter.
 * 
 */
QsvColorDefFactory::QsvColorDefFactory( QDomDocument doc )
{
	load( doc );
}

/**
 * \brief constructs a color factory with a document
 * \param fileName the file from which the color definitions should be loaded
 * 
 * Builds the factory, without and loading the definitions from the
 * xml file passed as the parameter.
 * 
 */
QsvColorDefFactory::QsvColorDefFactory( QString fileName )
{
	load( fileName );
}

/**
 * \brief default destructor
 * 
 * Kills this object. Does nothing but have an interface for derived 
 * classes.
 */
QsvColorDefFactory::~QsvColorDefFactory()
{
}

/**
 * \brief load color definitions from a QDomDocument
 * \return true on succes, false on ay error
 * 
 * Loads the color definitions from a QDomDocument. The document
 * should consist of single root elemment called (tag name) "itemDatas" and 
 * a list of elements called "itemData". The attributes of the node
 * will represent the values of the color defintion.
 * 
 * For more details look at QsvColorDef::load( QDomNode node )
 * 
 * \see QsvColorDef::load( QDomNode node )
 * \see load( QString )
 * \see getColorDef( QString )
 */
bool	QsvColorDefFactory::load( QDomDocument doc )
{
	// load the attributes of this language
	QDomNodeList list = doc.elementsByTagName("itemData");

	for( uint n=0; n<list.length(); n++ )
	{
		QsvColorDef item;
		if (item.load(list.item(n)))
			colorDefs.append( item );
	}

	return true;
}

/**
 * \brief load color definitions from a QDomDocument
 * \return true on succes, false on ay error
 * 
 * Loads the color definitions from an external XML file.
 * This is a convenience overloaded function which behaves like the above function.
 * 
 * \see load( QDomNode )
 */
bool	QsvColorDefFactory::load( QString fileName )
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly))
	{
		// TODO: display debug message, saying that
		// this is an invalid XML
		return false;
	}

	QDomDocument doc("language");
	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	return load( doc );
}

/**
 * \brief query the factory and return a color definition
 * \param name the color class to match
 * \return the color definition which matches the input
 *
 * When constructing this class, you will ask it to loads all
 * the color definitions from an XML file Then you will generally
 * ask for the color of the class "string" or something similar. 
 *
 * This factory does not care about the names, and they should 
 * match the names found on the color and syntax files definitions. 
 *
 * \see load( QDomDocument )
 * \see QsvColorDef
 * \see QsvColorDef::getStyleNum()
 */
QsvColorDef QsvColorDefFactory::getColorDef( QString name )
{
#ifdef __DEBUG_NO_ITEM_FOUND
	qDebug( "%s %d", __FILE__, __LINE__ );
#endif

	foreach(QsvColorDef color, colorDefs)
	{	
		if (color.getStyleNum() == name )
		{
#ifdef __DEBUG_NO_ITEM_FOUND__
	// new empthy one	
	qDebug( "%s %d - fond a color definition named %s - %s", __FILE__, __LINE__, 
		qPrintable(name), 
		qPrintable(color.toCharFormat().foreground().color().name()) 
	);
#endif				
			return color;
		}
	}

#ifdef __DEBUG_NO_ITEM_FOUND__
	// new empthy one	
	qDebug( "%s %d - could not find a color definition named (%s)", __FILE__, __LINE__, qPrintable(name) );
#endif	
	return QsvColorDef();
}
