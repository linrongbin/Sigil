/************************************************************************
**
**  Copyright (C) 2009, 2010, 2011  Strahinja Markovic  <strahinja.markovic@gmail.com>
**
**  This file is part of Sigil.
**
**  Sigil is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Sigil is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Sigil.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#pragma once
#ifndef IMPORTOEBPS_H
#define IMPORTOEBPS_H

#include "Importer.h"
#include "BookManipulation/Metadata.h"
#include <QHash>
#include <QStringList>

class HTMLResource;
class CSSResource;
class QDomDocument;
class QXmlStreamReader;

/**
 * The abstract base class for importers of OEBPS-like documents.
 */
class ImportOEBPS : public Importer
{

public:

    /**
     * Constructor.
     * 
     * @param fullfilepath The path to the file to be imported.
     */
    ImportOEBPS( const QString &fullfilepath );

    // inherited

    virtual ~ImportOEBPS();

    virtual QSharedPointer< Book > GetBook() = 0;

protected:
    
    /**
     * Extracts the EPUB file to a temporary folder.
     * The path to the the temp folder with the extracted files
     * is stored in m_ExtractedFolderPath.
     */
    void ExtractContainer();

    /**
     * Locates the OPF file in the extracted folder.
     * The path to the OPF is then stored in m_OPFFilePath.
     */
    void LocateOPF();
    
    /**
     * Parses the OPF file and stores the parsed information 
     * inside m_MetaElements, m_Files and m_ReadingOrderIds 
     */
    void ReadOPF();

    /**
     * Reads a Dublin Core element.
     *
     * @param opf_reader The OPF reader positioned to read 
     *                   the required element type.
     */
    void ReadDublinCoreElement( QXmlStreamReader &opf_reader );

    /**
     * Reads a regular <meta> element.
     *
     * @param opf_reader The OPF reader positioned to read 
     *                   the required element type.
     */
    void ReadRegularMetaElement( QXmlStreamReader &opf_reader );

    /**
     * Reads a manifest <item> element.
     *
     * @param opf_reader The OPF reader positioned to read 
     *                   the required element type.
     */
    void ReadManifestItemElement( QXmlStreamReader &opf_reader );

    /**
     * Reads a spine <item> element.
     *
     * @param opf_reader The OPF reader positioned to read 
     *                   the required element type.
     */
    void ReadSpineItemRefElement( QXmlStreamReader &opf_reader );

    /**
     * Reads a guide <reference> element.
     *
     * @param opf_reader The OPF reader positioned to read 
     *                   the required element type.
     */
    void ReadGuideReferenceElement( QXmlStreamReader &opf_reader );

    /**
     * Loads the metadata from the m_MetaElements list 
     * (filled by reading the OPF) into the book.
     */ 
    void LoadMetadata();

    /**
     * Loads the referenced files into the main folder of the book. 
     * @return A hash with keys being old references (URLs) to resources,
     *         and values being the new references to those resources.
     */ 
    QHash< QString, QString > LoadFolderStructure();

    /**
     * Loads a single file.
     * 
     * @param path A full path to the file to load.
     * @param reading_order 
     */
    tuple< QString, QString > LoadOneFile( const QString &path,
                                           int reading_order,
                                           const QHash< QString, QString > &semantic_info );


    ///////////////////////////////
    // PROTECTED MEMBER VARIABLES
    ///////////////////////////////
    
    /**
     * The full path to the folder where the 
     * EPUB was extracted to.
     */
    QString m_ExtractedFolderPath;

    /**
     * The full path to the OPF file 
     * of the publication.
     */ 
    QString m_OPFFilePath;

    /**
     * The map of all the files in the publication's 
     * manifest; The keys are the element ID's, 
     * the values are stored paths to the files.
     */ 
    QMap< QString, QString > m_Files;

    /**
     * InDesign likes listing several files multiple times in the manifest, 
     * even though that's explicitly forbidden by the spec. So we use this
     * to make sure we don't load such files multiple times.
     */
    QSet< QString > m_MainfestFilePaths;

    /**
     * The list of ID's to the files in the manifest
     * that represent the reading order of the publication.
     */ 
    QStringList m_ReadingOrderIds;

    /**
     * The identifier of the book's unique identifier.
     */
    QString m_UniqueIdentifierId;

    /**
     * The list of metadata elements in the OPF. 
     */ 
    QList< Metadata::MetaElement > m_MetaElements;

    /**
     * The keys are the file ID's, the values  
     * are key-value pairs of semantic information.
     */ 
    QHash< QString, QHash< QString, QString > > m_SemanticInformation;
};


#endif // IMPORTOEBPS_H
