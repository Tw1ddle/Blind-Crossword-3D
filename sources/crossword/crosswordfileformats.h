/*! \brief The crossword file formats.
 *  These file formats are meant for use by the loader classes. They match the file extensions that should be used when saving files of a given format.
 */

#pragma once

namespace fileformat {

typedef QString FORMAT;
const FORMAT XWC3D = "xwc3d";
const FORMAT XWCR3D = "xwcr3d";
const FORMAT XWC = "xwc";

}
