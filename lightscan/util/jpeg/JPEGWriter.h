#ifndef AJS_JPEG_WRITER_H
#define AJS_JPEG_WRITER_H

#include <cstdio>
#include <cstdlib>
#include <cassert>

extern "C" {
    #include <jpeglib.h>
    #include <jerror.h>
}

#include <vector>
#include <string>
#include <stdexcept>

#include "JPEG.h"

/// \class JPEGWriter JPEGWriter.h
/// Write a JPEG image to file using \c libjpeg.
/// Thread-safe if multiple threads do not access the same JPEGWriter at once.
/// Writes directly from the caller's memory, minimizing copying.
class JPEGWriter {
    public:

    /// Initialize the libjpeg structures.
    JPEGWriter();

    /// Free the libjpeg structures.
    ~JPEGWriter();

    /// Set up the JPEG header,
    void header(const unsigned width, const unsigned height,
                const unsigned components, const JPEG::ColorSpace colorSpace);

    /// Set the quality setting, in the range of zero (lowest) to 100 (highest).
    /// If \c forceBaseline is true, then compatibility with different JPEG
    /// readers is increased at the expense of increased file size of very-low
    /// quality (under 25) images.
    void setQuality(const unsigned value, const bool forceBaseline = false);

    /// Set the time/quality tradeoff.
    void setTradeoff(const JPEG::TimeQualityTradeoff value);

    /// Write a JPEG image given by the row iterator \c rows to the file at \c path.
    ///
    /// A RowPtrIter should act basically like an \c unsigned char**:
    ///
    /// - It must dereference to the start of a chunk of memory at least
    ///   \c width * \c components bytes long.
    /// - It should be incrementable \c height times.
    /// - It will be pre-incremented immediately after each row is written out.
    ///
    /// Normal pointers, iterators into a std::vector<unsigned char*>, and related
    /// things will all work as you expect.
    ///
    /// A normal usage would be something like:
    /// \code
    /// writer.header(width, height, 3, JPEG::COLOR_RGB);
    /// unsigned char** rows = new unsigned char*[height];
    /// for (int i = 0; i < height; ++i)
    ///     rows[i] = new unsigned char[width];
    /// writer.write("myfile.jpg", rows);
    /// \endcode
    /// Of course, the equivalent code using \c std::vector is highly recommended.
    ///
    /// If you have only a row at a time, then something like the following
    /// will also work and be more efficient:
    /// \code
    /// struct InputRowIter {
    ///     InputRowIter(some input source, const unsigned size):
    ///         buffer(size) {
    ///         ... fill the buffer with one row's worth of pixels ...
    ///     }
    ///
    ///     unsigned char* operator*() {
    ///         return &buffer[0];
    ///     }
    ///
    ///     void operator++() {
    ///         ... fill the buffer with one row's worth of pixels ...
    ///     }
    ///
    ///     std::vector<unsigned char> buffer;
    /// };
    ///
    /// InputRowIter rowIter(some input source, width * components);
    /// writer.header(width, height, 3, JPEG::COLOR_RGB);
    /// writer.write("myfile.jpg", rowIter);
    /// \endcode
    template <typename RowPtrIter>
    void write(const std::string& path, RowPtrIter rows);

    template <typename RowPtrIter>
    void write_mem(char **output, size_t *len, RowPtrIter rows);

    /// Get warnings generated by libjpeg since the last call to header().
    /// Separate warnings are separated by a newline.
    const std::string& warnings() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /// libjpeg wants us to exit because of an error (private).
    void error_exit();

    /// libjpeg wants us to output a message because of an error (private).
    void output_message();
#endif

    private:
    // Disallow copying
    JPEGWriter(const JPEGWriter& other);
    JPEGWriter& operator=(const JPEGWriter& other);

    struct jpeg_compress_struct cinfo;          /// libjpeg file structure
    struct jpeg_error_mgr jerr;                 /// libjpeg error structure

    std::string warningMsg;                     /// All the warnings.
};

inline void JPEGWriter::setQuality(const unsigned value, const bool forceBaseline) {
    assert(value >= 0 && value <= 100);
    jpeg_set_quality(&cinfo, value, forceBaseline);
}

inline const std::string& JPEGWriter::warnings() const {
    return warningMsg;
}

template <typename RowPtrIter>
void JPEGWriter::write(const std::string& path, RowPtrIter rows) {

  // Specify the destination for the compressed data (eg, a file)
  FILE* file = NULL;
  if ((file = fopen(path.c_str(), "wb")) == NULL)
    throw std::runtime_error("Cannot open " + path);

  jpeg_stdio_dest(&cinfo, file);

  jpeg_start_compress(&cinfo, true);

  while (cinfo.next_scanline < cinfo.image_height) {
    unsigned char* row_ptr = *rows;
    jpeg_write_scanlines(&cinfo, &row_ptr, 1);
    ++rows;
  }

  jpeg_finish_compress(&cinfo);

  fclose(file);
}

template <typename RowPtrIter>
void JPEGWriter::write_mem(char **output,
                           size_t *len,
                           RowPtrIter rows) {
  unsigned char* outp = NULL;

  size_t out_len = 0;

  jpeg_mem_dest(&cinfo, &outp, &out_len);

  jpeg_start_compress(&cinfo, true);

  while (cinfo.next_scanline < cinfo.image_height) {
    unsigned char* row_ptr = *rows;
    jpeg_write_scanlines(&cinfo, &row_ptr, 1);
    ++rows;
  }

  jpeg_finish_compress(&cinfo);

  *output = (char*)outp;
  *len = out_len;
}


#endif
