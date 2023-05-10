#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>

/*
Original code by dascandy, reverse engineering work colab between BlackStar & dascandy / Edge of Panic
*/

struct Chunk {
    uint32_t start;
    uint32_t end;
    uint16_t id;

    uint32_t length() const
    {
        return end - start;
    }
};

struct ImageDecoder {
  uint8_t* in;
  uint32_t length;
  uint8_t* out;
  uint8_t* lastline;
  uint8_t buffer[4096];
  uint8_t nibblebuffer;
  uint16_t bitbuffer;
  uint8_t bits = 0;
  bool hasNibble = false;
  uint8_t offset = 0;
  uint8_t bxoffset = 0;
  uint16_t width = 0;
  static constexpr int deltas[] = { -1, -2, -4, -8, 1, 0 };
  uint8_t loadnibble() {
    if (hasNibble) {
      hasNibble = false;
      return (nibblebuffer >> 4);
    } else {
      hasNibble = true;
      nibblebuffer = *in++;
      return nibblebuffer & 0xF;
    }
  }
  void start() {
    bitbuffer = *in++;
    bitbuffer |= (*in++) << 8;
    bits = 16;
  }
  uint8_t getbit() {
    uint8_t rv = (bitbuffer & 0x8000) ? 1 : 0;
    bitbuffer = bitbuffer << 1;
    bits--;
    if (not bits) {
      bitbuffer = *in++;
      bitbuffer |= (*in++) << 8;
      bits = 16;
    }
    return rv;
  }
  enum HuffmanCodes {
    SkipSingle,
    CopyFromBxTable,
    CopySkipTable,
    CopyMoveTable,
    CopyFromBack,
    CopyAndStore,
  };
  HuffmanCodes getHuffmanCode() {
    if (getbit()) {
      if (getbit()) {
        if (getbit()) {
          if (getbit()) {
            return CopyFromBxTable;
          } else {
            return CopyMoveTable;
          }
        } else {
          return CopyAndStore;
        }
      } else {
        return SkipSingle;
      }
    } else {
      if (getbit()) {
        return CopySkipTable;
      } else {
        return CopyFromBack;
      }
    }
  }
  void handle_one() {
    switch(getHuffmanCode()) {
    case SkipSingle:
      printf("SkipSingle %d\n", offset);
      offset++;
      break;
    case CopyAndStore:
      printf("CopyAndStore %d\n", offset);
      out[offset*4] = buffer[bxoffset*4] = *in++;
      out[offset*4+1] = buffer[bxoffset*4+1] = *in++;
      out[offset*4+2] = buffer[bxoffset*4+2] = *in++;
      out[offset*4+3] = buffer[bxoffset*4+3] = *in++;
      bxoffset++;
      offset++;
      break;
    case CopyFromBxTable:
    {
      printf("CopyFromBxTable %d\n", offset);
      uint8_t v = *in++;
      out[offset*4] = buffer[v*4];
      out[offset*4+1] = buffer[v*4+1];
      out[offset*4+2] = buffer[v*4+2];
      out[offset*4+3] = buffer[v*4+3];
      offset++;
    }
      break;
    case CopySkipTable:
    {
      uint8_t v = loadnibble();
      printf("CopySkipTable %d %d\n", offset, v);
      if (v == 0) {
        uint8_t v2 = loadnibble();
        out[offset*4] = (v2 & 1) ? 0xFF : 0;
        out[offset*4+1] = (v2 & 2) ? 0xFF : 0;
        out[offset*4+2] = (v2 & 4) ? 0xFF : 0;
        out[offset*4+3] = (v2 & 8) ? 0xFF : 0;
        offset++;
      } else if (v == 15) {
        out[offset*4] = lastline[offset*4];
        out[offset*4+1] = lastline[offset*4+1];
        out[offset*4+2] = lastline[offset*4+2];
        out[offset*4+3] = lastline[offset*4+3];
        offset++;
      } else {
        for (size_t n = 0; n < 4; n++) {
          if ((v >> n) & 1) {
            out[offset*4+n] = *in++;
          }
        }
        offset++;
      }
    }
      break;
    case CopyMoveTable:
    {
      uint8_t v = loadnibble();
      printf("CopyMoveTable %d %d\n", offset, v);
      if (v == 0) {
        uint8_t count = *in++;
        int delta = deltas[count >> 6];
        count = ((count & 0x3F) + 0x12);
        for (size_t n = 0; n < count; n++) {
          out[offset*4] = out[delta*4 + offset*4];
          out[offset*4+1] = out[delta*4 + offset*4+1];
          out[offset*4+2] = out[delta*4 + offset*4+2];
          out[offset*4+3] = out[delta*4 + offset*4+3];
          offset++;
        }
      } else if (v == 15) {
        uint8_t count = *in++;
        if ((count & 0xC0) == 0) {
          count = ((count & 0x3F) + 0x12);
          for (size_t n = 0; n < count; n++) {
            out[offset*4] = lastline[offset*4];
            out[offset*4+1] = lastline[offset*4+1];
            out[offset*4+2] = lastline[offset*4+2];
            out[offset*4+3] = lastline[offset*4+3];
            offset++;
          }
        } else {
          count = ((count & 0x3F) + 0x12);
          offset += count;
        }
      } else {
        for (size_t n = 0; n < 4; n++) {
          if ((v >> n) & 1) {
            out[offset*4+n] = *in++;
          } else {
            out[offset*4+n] = out[offset*4+n - 4];
          }
        }
        offset++;
      }
    }
      break;
    case CopyFromBack:
    {
      uint8_t a = loadnibble();
      printf("CopyFromBack %d %d\n", offset, a);
      if (a < 4) {
        int delta = deltas[a];
        out[offset*4] = out[offset*4+delta*4];
        out[offset*4+1] = out[offset*4+delta*4+1];
        out[offset*4+2] = out[offset*4+delta*4+2];
        out[offset*4+3] = out[offset*4+delta*4+3];
        offset++;
      } else {
        uint8_t count;
        int delta;
        if (a < 10) {
          count = loadnibble() + 2;
          delta = deltas[a-4];
        } else {
          count = width - offset;
          delta = deltas[a-10];
        }
        if (delta < 0) {
          for (size_t k = 0; k < count; k++) {
            out[offset*4] = out[delta*4 + offset*4];
            out[offset*4+1] = out[delta*4 + offset*4+1];
            out[offset*4+2] = out[delta*4 + offset*4+2];
            out[offset*4+3] = out[delta*4 + offset*4+3];
            offset++;
          }
        } else if (delta > 0) {
          for (size_t k = 0; k < count; k++) {
            out[offset*4] = lastline[offset*4];
            out[offset*4+1] = lastline[offset*4+1];
            out[offset*4+2] = lastline[offset*4+2];
            out[offset*4+3] = lastline[offset*4+3];
            offset++;
          }
        } else {
          offset += count;
        }
      }
    }
      break;
    }
  }
};

const uint8_t bmpheader[] = {0x42, 0x4d, 0x0a, 0xc2, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x00, 0x00, 0x00, 0x7c, 0x00, 
                             0x00, 0x00, 0x50, 0x02, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
                             0x00, 0x00, 0x80, 0xc1, 0x09, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x00, 0x00, 
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t defaultpal[] =
{
 0x00, 0x00, 0x0c, 0x00, 0xc0, 0x00, 0xfa, 0x0d, 0x00, 0x0c, 0x0c, 0x0c, 0xc0, 0x0c, 0x77, 0x07,
 0xcc, 0x0c, 0x0f, 0x00, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0xf0, 0x0f, 0xff, 0x0f,
};

int main(int argc, char** argv) {
  std::vector<uint8_t> in;
  std::vector<uint8_t> line1, line2;
  std::vector<uint8_t> bmp;
  line1.resize(0x280);
  
#ifdef _DEBUG
  const char* file = "3.GC";
#else
  char* file = argv[1];
#endif
  
  in.resize(std::filesystem::file_size(file));
  
  std::string output = file;
  

  std::ifstream gc_in(file, std::ios::binary);
  if (!gc_in.is_open())
      return -1;

  gc_in.read((char*)in.data(), in.size());
  bool haspal = *(in.data() + 0x04) == 0x80;
  uint16_t num_chunks = *((uint16_t*)(in.data() + 0x08));
  uint32_t chunk_table = haspal ? 0x30 : 0x10;

  std::vector<Chunk> chunks;
  for (uint16_t c = 0; c < num_chunks; ++c)
  {
      uint32_t start = *(uint32_t*)(in.data() + chunk_table + (sizeof(uint32_t) * c));
      uint32_t end = *(uint32_t*)(in.data() + chunk_table + (sizeof(uint32_t) * (c + 1)));
      Chunk chunk;
      chunk.start = start;
      chunk.end = end;
      chunk.id = c;
      chunks.push_back(chunk);
  }


  for (auto& chunk : chunks)
  {
      bmp.clear();

      std::stringstream ss;

      ss << output << "_" << chunk.id << ".BMP";
      std::string outputfile = ss.str();

      ImageDecoder dec;
      uint32_t offset = chunk.start + 10;
      dec.in = in.data() + offset;
      dec.length = chunk.length();
      dec.out = line1.data();
      dec.lastline = line1.data() + 0x140;


      dec.width = (*(in.data() + 0x3d) >> 1);
      uint16_t width = 592 / 8;

      dec.start();

      for (size_t n = 0; n < 360; n++) {
          dec.offset = 0;
          while (dec.offset < dec.width) {
              dec.handle_one();
          }
          std::vector<uint8_t> linebuf;
          for (size_t n = 0; n < width; n++) {
              for (int b = 7; b >= 0; b--) {
                  uint8_t val = 0;
                  for (size_t x = 0; x < 4; x++) {
                      if (dec.out[n * 4 + x] & (1 << b)) val |= (1 << x);
                  }
                  uint8_t color1;
                  uint8_t color2;

                  if (haspal)
                  {
                      color1 = in[val * 2 + 0x10];
                      color2 = in[val * 2 + 0x11];
                  }
                  else
                  {
                      color1 = defaultpal[val * 2];
                      color2 = defaultpal[(val * 2) + 1];
                  }

                  linebuf.push_back((color1 & 0xF) << 4);
                  linebuf.push_back(color2 << 4);
                  linebuf.push_back(color1 & 0xF0);

              }
          }
          std::reverse(linebuf.begin(), linebuf.end());
          bmp.insert(bmp.end(), linebuf.begin(), linebuf.end());
          std::swap(dec.out, dec.lastline);
      }
      std::reverse(bmp.begin(), bmp.end());

      std::ofstream out(outputfile, std::ios::binary);
      out.write((const char*)bmpheader, sizeof(bmpheader));
      out.write((const char*)bmp.data(), bmp.size());
  }
}
