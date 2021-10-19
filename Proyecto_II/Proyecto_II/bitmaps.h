
// ---------------------------- Proyecto --------------------------
unsigned char ladrillo_gris[] { // 5x5
 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 
0x5a, 0xcb, 0x5a, 0xab, 0x6b, 0x6d, 0x84, 0x10, 0x84, 0x10, 
0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 
0x84, 0x10, 0x84, 0x10, 0x6b, 0x4d, 0x5a, 0xab, 0x5a, 0xcb, 
0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 
};

unsigned char cuadro_vida[]{
  0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 
0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 
0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 
0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 
0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 0xf0, 0xe4, 
};
unsigned char J1[]{ // back, derecha
  0x00, 0x00, 0x00, 0x00, 0xa1, 0x40, 0x91, 0x00, 0x91, 0x00, 0x91, 0x00, 0x91, 0x00, 0xa1, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0xc0, 0x99, 0x80, 0x99, 0x80, 0x91, 0x00, 0x91, 0x00, 0xa1, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0xc0, 0x99, 0xa0, 0x99, 0xa0, 0x99, 0xa0, 0x99, 0xa0, 0xa9, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xb1, 0x20, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfe, 0xff, 0xff, 0xa8, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x99, 0x80, 0x91, 0x20, 0x88, 0xc0, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xc1, 0x60, 0x00, 0x00, 0x00, 0x00, 0xb1, 0x60, 0x99, 0x80, 0x99, 0x80, 0x99, 0xa0, 0x99, 0xa0, 0x99, 0xa0, 0x99, 0x80, 0xb1, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xfa, 0x00, 0x00, 0xff, 0xfc, 0xff, 0xfb, 0x00, 0x00, 0xff, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xa9, 0xc0, 0x90, 0xe0, 0xff, 0xff, 0x78, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x88, 0xa0, 0x99, 0x80, 0x99, 0x80, 0x99, 0x80, 0x99, 0x80, 0x80, 0x60, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xba, 0xff, 0xfc, 0xff, 0x99, 0xff, 0xba, 0xff, 0xfc, 0xff, 0x99, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x90, 0xe0, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0x99, 0xff, 0xfc, 0xff, 0x99, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x88, 0xa0, 0x91, 0x00, 0x91, 0x00, 0x80, 0x60, 0xff, 0xfe, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xfc, 0xff, 0xfc, 0xff, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb1, 0x40, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xfc, 0xff, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x04, 0xdf, 0x04, 0xdf, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x04, 0xdf, 0x04, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x04, 0x9f, 0x04, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x04, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xa0, 0x04, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x04, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x08, 0x21, 0x00, 0x01, 0xf5, 0x81, 0xed, 0x20, 0x00, 0x01, 0x00, 0x01, 0xf5, 0x20, 0xf5, 0xa1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf5, 0xa1, 0x00, 0x00, 0x08, 0x21, 0x00, 0x01, 0xed, 0x20, 0xf5, 0xa1, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xed, 0x61, 0xf5, 0x20, 0x00, 0x01, 0x00, 0x01, 0xe5, 0x00, 0xf5, 0x81, 0x00, 0x21, 0x08, 0x21, 
};

unsigned char J2[]{
  0x00, 0x00, 0x00, 0x00, 0xf7, 0x9e, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xf7, 0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x9e, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xf7, 0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x9e, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xf7, 0x9e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xd9, 0xff, 0xfa, 0xff, 0xb9, 0xff, 0xb9, 0xff, 0xfa, 0xff, 0xd9, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xbb, 0xff, 0xd9, 0xff, 0xfa, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xfc, 0xff, 0xfb, 0xf8, 0x00, 0xff, 0xfc, 0xff, 0xfb, 0xf8, 0x00, 0xff, 0xfc, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xf7, 0xbe, 0xde, 0xdb, 0xff, 0xd9, 0xd6, 0x9c, 0xff, 0xfc, 0xf8, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0xd6, 0xbb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xde, 0xdb, 0xd6, 0xbb, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xba, 0xff, 0xfc, 0xff, 0xb9, 0xff, 0xd9, 0xff, 0xfc, 0xff, 0x99, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xde, 0xdb, 0xff, 0xb9, 0xff, 0xb9, 0xff, 0x99, 0xff, 0xfc, 0xff, 0xb9, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xd9, 0xd6, 0xbb, 0xde, 0xdb, 0xde, 0xdb, 0xd6, 0xbb, 0xff, 0xb9, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x85, 0xbd, 0x8d, 0xfd, 0x8d, 0xfd, 0x8d, 0xdd, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xd9, 0xff, 0xb9, 0xff, 0xd9, 0x85, 0xbd, 0xb7, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xd9, 0xff, 0xd9, 0xff, 0xd9, 0xff, 0xd9, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xbf, 0xbe, 0x3d, 0xbe, 0x5d, 0xe7, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, 0x9f, 0xae, 0x1d, 0xae, 0x1d, 0xe7, 0xbf, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd7, 0x7f, 0xae, 0x1d, 0xae, 0x1d, 0xdf, 0x9f, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xd7, 0x1f, 0xce, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xd7, 0x1f, 0xd7, 0x1f, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xce, 0xff, 0xd7, 0x1f, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xae, 0x7f, 0xae, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xae, 0x7f, 0xae, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xae, 0x7f, 0xae, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0xff, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xbf, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xbe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xbe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xde, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x7d, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf7, 0x7d, 0x00, 0x00, 0x00, 0x00, 0xef, 0x5c, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
};
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
