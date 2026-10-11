#pragma once

// Kiểm tra trình biên dịch đang đọc chuỗi tiếng Việt đúng chuẩn UTF-8.
// Nếu lỗi biên dịch ở dòng static_assert dưới đây, hãy bật /utf-8 trong Visual Studio:
// Project > Properties > C/C++ > Command Line > Additional Options > thêm: /utf-8
static_assert(sizeof("ạ") == 4 &&
              static_cast<unsigned char>("ạ"[0]) == 0xE1 &&
              static_cast<unsigned char>("ạ"[1]) == 0xBA &&
              static_cast<unsigned char>("ạ"[2]) == 0xA1,
              "Chuoi tieng Viet khong phai UTF-8. Hay bat /utf-8 (C/C++ > Command Line > Additional Options).");
