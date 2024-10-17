char get_color_word(unsigned char background, unsigned char foreground) {
  return background << 4 | foreground & 0x0F;
}
