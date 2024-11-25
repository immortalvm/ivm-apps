#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ivmfs.c"

typedef struct File {
  char *ptr;
  size_t len;
  size_t cap;
} File;

static int readEntireFileAlloc(const char *path, File *out) {
  FILE *file = fopen(path, "r");
  size_t bytes_read = 0;
  size_t total_bytes_read = 0;
  do {
    bytes_read = fread((void *)&out->ptr[total_bytes_read], 1,
                       out->cap - 1 - total_bytes_read, file);
    total_bytes_read += bytes_read;
  } while (bytes_read != 0);
  out->len = total_bytes_read;
  return 0;
}

static char file_memory[8 * 1024 * 1024];
#define global_new_file                                                        \
  (File) { .ptr = file_memory, .len = 0, .cap = sizeof(file_memory) }

typedef struct WavHeader {
  uint8_t FileTypeBlocID[4];
  uint32_t FileSize;
  uint8_t FileFormatID[4];
  uint8_t FormatBlocID[4];
  uint32_t BlocSize;
  uint16_t AudioFormat;
  uint16_t NbrChannels;
  uint32_t Frequency;
  uint32_t BytePerSec;
  uint16_t BytePerBloc;
  uint16_t BitsPerSample;
  uint8_t DataBlocID[4];
  uint32_t DataSize;
  uint16_t SampleData;
} WavHeader;

static WavHeader *print_wav_header(File file) {
  if (file.len < sizeof(WavHeader))
    return NULL;
  WavHeader *header = (WavHeader *)file.ptr;

  char buf[1024];
  size_t i = 0;

  i += (size_t)sprintf(&buf[i], "RIFF:\nFileTypeBlocID\t: '%.*s'\n", 4,
                       (char *)&header->FileTypeBlocID);
  i += (size_t)sprintf(&buf[i], "FileSize\t: %u\n", header->FileSize);
  i += (size_t)sprintf(&buf[i], "FileFormatID\t: '%.*s'\n\n", 4,
                       (char *)&header->FileFormatID);
  i += (size_t)sprintf(&buf[i], "fmt:\nFormatBlocID\t: '%.*s'\n", 4,
                       (char *)&header->FormatBlocID);
  i += (size_t)sprintf(&buf[i], "BlockSize\t: %u\n", header->BlocSize);
  i += (size_t)sprintf(&buf[i], "AudioFormat\t: %s\n",
                       header->AudioFormat == 1   ? "PCM integer"
                       : header->AudioFormat == 3 ? "IEEE 754 float"
                                                  : "(?)");
  i += (size_t)sprintf(&buf[i], "NbrChannels\t: %u\n", header->NbrChannels);
  i += (size_t)sprintf(&buf[i], "Frequency\t: %u\n", header->Frequency);
  i += (size_t)sprintf(&buf[i], "BytePerSec\t: %u\n", header->BytePerSec);
  i += (size_t)sprintf(&buf[i], "BytePerBloc\t: %u\n", header->BytePerBloc);
  i += (size_t)sprintf(&buf[i], "BitsPerSample\t: %u\n\n",
                       header->BitsPerSample);
  i += (size_t)sprintf(&buf[i], "data:\nDataBlocID\t: '%.*s'\n", 4,
                       (char *)&header->DataBlocID);
  i += (size_t)sprintf(&buf[i], "DataSize\t: %u\n\n", header->DataSize);

  if (sizeof(buf) < i)
    return NULL;

  fprintf(stdout, "%.*s", (int)i, buf);
  return header;
}

void ivm64_add_sample(long left, long right) {
#ifdef __ivm64__
  asm volatile("add_sample* [(load8 %[l]) (load8 %[r])]"
               :
               : [l] "m"(left), [r] "m"(right));
#else
  printf("add_sample* [%ld %ld]\n", left, right);
#endif
}

void ivm64_new_frame(long width, long height, long rate) {
#ifdef __ivm64__
  asm volatile("new_frame* [(load8 %[w]) (load8 %[h]) (load8 %[r])]"
               :
               : [w] "m"(width), [h] "m"(height), [r] "m"(rate));
#else
  printf("new_frame* [%ld %ld %ld]\n", width, height, rate);
#endif
}

int main(void) {
  for (size_t i = 0; i < filesystem->data.nfiles; i++) {
    const char *name = filesystem->data.filetable[i].name;
    if (strcmp(name, "/work/input.wav") == 0) {
      File cantina_band = global_new_file;
      if (readEntireFileAlloc(name, &cantina_band) != 0) {
        printf("Failed to read file\n");
        return 1;
      }
      printf("read %lu bytes from %s\n", cantina_band.len, name);
      WavHeader *wav = print_wav_header(cantina_band);
      ivm64_new_frame(0, 0, 22050);
      for (size_t i = 0; i < wav->DataSize; i++) {
        uint16_t *ptr = ((uint16_t *)(&wav->DataSize + 1)) + i;
        ivm64_add_sample(*ptr, *ptr);
      }
    }
  }
  return 0;
}
