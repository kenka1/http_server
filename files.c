#include "files.h"

const unsigned char index_html[] =
"<html><head><title>ESP32</title>"
"<link rel=\"stylesheet\" href=\"/style.css\"></head>"
"<body><h1>Hello ESP32</h1></body></html>";
const size_t index_html_len = sizeof(index_html) - 1;

const unsigned char style_css[] =
"body { background:#111; color:#0f0; font-family: monospace; }";
const size_t style_css_len = sizeof(style_css) - 1;

const unsigned char image_bin[] = { 0xDE, 0xAD, 0xBE, 0xEF };
const size_t image_bin_len = sizeof(image_bin);
