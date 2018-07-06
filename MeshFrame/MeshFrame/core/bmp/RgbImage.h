/*!
*      \file RgbImage.h
*      \brief BitMap Image
*      \date Documented on 10/08/2010
*
*/
#ifndef RGBIMAGE_H
#define RGBIMAGE_H

#include <stdio.h>
#include <assert.h>

// Include the next line to turn off the routines that use OpenGL
// #define RGBIMAGE_DONT_USE_OPENGL
/*!
 *	\brief RgbImage class
 *
 *  24 bit bmp image class.
 */
class RgbImage
{
public:
	/*!	RgbImage constructor
	*/
	RgbImage();
	/*!	RgbImage constructor
	* \param filename the input file nmae
	*/
	RgbImage( const char* filename );
	/*! RgbImage constructor
	*	\param numRows number of rows
	*   \param numCols number of cols
	*/
	RgbImage( int numRows, int numCols );	// Initialize a blank bitmap of this size.
	/*!	RgbImage destructor
	 *
	 */
	~RgbImage();
	
	/*!
	 *	LoadBmpFile 
	 * \param filename input file name
	 * \return whether the loading is successful
	 */
	bool LoadBmpFile( const char *filename );		// Loads the bitmap from the specified file
	/*!
	 *	WriteBmpFile 
	 * \param filename output file name
	 * \return whether the writing is successful
	 */
	bool WriteBmpFile( const char* filename );		// Write the bitmap to the specified file
#ifndef RGBIMAGE_DONT_USE_OPENGL
	/*! Load the Bmp Image from OpenGL buffer
	*  \return whether the loading is successful
	*/
	bool LoadFromOpenglBuffer();					// Load the bitmap from the current OpenGL buffer
#endif

	/*!	number of rows
	*/
	long GetNumRows() const { return NumRows; }
	/*!	number of columns
	*/
	long GetNumCols() const { return NumCols; }
	// Rows are word aligned
	/*! Get number of bytes per row
	 */	
	long GetNumBytesPerRow() const { return ((3*NumCols+3)>>2)<<2; }	
	/*!	Get image data buffer
	*/
	const void* ImageData() const { return (void*)ImagePtr; }
	/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \return array of unsigned char for the pixel color
	 */
	const unsigned char* GetRgbPixel( long row, long col ) const;
	/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \return array of unsigned char for the pixel color
	 */
	unsigned char* GetRgbPixel( long row, long col );
	/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \param red pointer to the red color
	 * \param green pointer to the green color
	 * \param blue  pointer to the blue  color
	 */
	void GetRgbPixel( long row, long col, float* red, float* green, float* blue ) const;
	/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \param red pointer to the red color
	 * \param green pointer to the green color
	 * \param blue  pointer to the blue  color
	 */
	void GetRgbPixel( long row, long col, double* red, double* green, double* blue ) const;

	/*! set the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \param red   the red color floating point
	 * \param green the green color floating point
	 * \param blue  the blue  color floating point
	 */
	void SetRgbPixelf( long row, long col, double red, double green, double blue );

	/*! set the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \param red   the red color unsigned char
	 * \param green the green color unsigned char
	 * \param blue  the blue  color unsigned char
	 */
	void SetRgbPixelc( long row, long col, 
					   unsigned char red, unsigned char green, unsigned char blue );
	/*! get the error code
	 */

	// Error reporting. (errors also print message to stderr)
	int GetErrorCode() const { return ErrorCode; }
	enum {
		NoError = 0,
		OpenError = 1,			// Unable to open file for reading
		FileFormatError = 2,	// Not recognized as a 24 bit BMP file
		MemoryError = 3,		// Unable to allocate memory for image data
		ReadError = 4,			// End of file reached prematurely
		WriteError = 5			// Unable to write out data (or no date to write out)
	};
	bool ImageLoaded() const { return (ImagePtr!=0); }  // Is an image loaded?

	void Reset();			// Frees image data memory

private:
	/*!	
	pixel buffer
	*/
	unsigned char* ImagePtr;	// array of pixel values (integers range 0 to 255)
	/*! number of rows
	*/
	long NumRows;				// number of rows in image
	/*! number of columns
	*/
	long NumCols;		// number of columns in image
	/*! current error code
	*/
	int ErrorCode;				// error code
	
	/*!	read a short integer form a file
	\param infile input file
	\return short integer just read
	*/
	static short readShort( FILE* infile );
	/*!	read a long integer form a file
	\param infile input file
	\return long integer just read
	*/
	static long readLong( FILE* infile );
	/*!	skip several chars in the file
	\param infile input file name
	\param numChars the number of chars to be skipped
	*/
	
	static void skipChars( FILE* infile, int numChars );
	/*!	write a long integer to the file
	\param data the long integter to write
	\param outfile output file
	*/
	static void RgbImage::writeLong( long data, FILE* outfile );
	/*!	write a short integer to the file
	\param data the short integter to write
	\param outfile output file
	*/
	static void RgbImage::writeShort( short data, FILE* outfile );
	
	/*!	convert a double to unsigned char
	* \param x input double number
	* \return output unsigned char
	*/
	static unsigned char doubleToUnsignedChar( double x );

};

/*!	RgbImage constructor
	*/
inline RgbImage::RgbImage()
{ 
	NumRows = 0;
	NumCols = 0;
	ImagePtr = 0;
	ErrorCode = 0;
}
/*!	RgbImage constructor
	* \param filename the input file nmae
	*/
inline RgbImage::RgbImage( const char* filename )
{
	NumRows = 0;
	NumCols = 0;
	ImagePtr = 0;
	ErrorCode = 0;
	LoadBmpFile( filename );
}
/*!	RgbImage destructor
	 *
	 */
inline RgbImage::~RgbImage()
{ 
	delete[] ImagePtr;
}

// Returned value points to three "unsigned char" values for R,G,B
/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \return array of unsigned char for the pixel color
	 */
inline const unsigned char* RgbImage::GetRgbPixel( long row, long col ) const
{
	assert ( row<NumRows && col<NumCols );
	const unsigned char* ret = ImagePtr;
	long i = row*GetNumBytesPerRow() + 3*col;
	ret += i;
	return ret;
}
/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \return array of unsigned char for the pixel color
	 */
inline unsigned char* RgbImage::GetRgbPixel( long row, long col ) 
{
	assert ( row<NumRows && col<NumCols );
	unsigned char* ret = ImagePtr;
	long i = row*GetNumBytesPerRow() + 3*col;
	ret += i;
	return ret;
}
/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \param red pointer to the red color
	 * \param green pointer to the green color
	 * \param blue  pointer to the blue  color
	 */
inline void RgbImage::GetRgbPixel( long row, long col, float* red, float* green, float* blue ) const
{
	assert ( row<NumRows && col<NumCols );
	const unsigned char* thePixel = GetRgbPixel( row, col );
	const float f = 1.0f/255.0f;
	*red = f*(float)(*(thePixel++));
	*green = f*(float)(*(thePixel++));
	*blue = f*(float)(*thePixel);
}
/*! Get the pixel at (row, col)
	 * \param row row position
	 * \param col column position
	 * \param red pointer to the red color
	 * \param green pointer to the green color
	 * \param blue  pointer to the blue  color
	 */
inline void RgbImage::GetRgbPixel( long row, long col, double* red, double* green, double* blue ) const
{
	assert ( row<NumRows && col<NumCols );
	const unsigned char* thePixel = GetRgbPixel( row, col );
	const double f = 1.0/255.0;
	*red = f*(double)(*(thePixel++));
	*green = f*(double)(*(thePixel++));
	*blue = f*(double)(*thePixel);
}
/*! reset the RgbImage object
*/
inline void RgbImage::Reset()
{
	NumRows = 0;
	NumCols = 0;
	delete[] ImagePtr;
	ImagePtr = 0;
	ErrorCode = 0;
}


#endif // RGBIMAGE_H
