/*! \brief The crossword file formats.
 *
 *  These file formats are meant for use by the loader classes. They match the file extensions that should be used when saving files of a given format.
 *
 *
 *  \author Samuel Twidale
 *  \copyright GNU General Public License v3.0
 */

#pragma once

namespace FileFormats
{
    typedef QString FORMAT;

    const FORMAT XWC3D = "xwc3d";
    const FORMAT XWCR3D = "xwcr3d";
    const FORMAT XWC = "xwc";
}
