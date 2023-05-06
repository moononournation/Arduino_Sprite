class Sprite
{
public:
  Sprite(int16_t x, uint16_t y, uint8_t *bitmap, uint16_t *palette, int16_t w, int16_t h, bool loop, size_t frames)
      : _x(x), _y(y), _w(w), _h(h), _loop(loop), _frames(frames)
  {
    init_bitmap(bitmap, palette, w * h * frames);
  }
  Sprite(int16_t x, uint16_t y, uint8_t *bitmap, uint16_t *palette, int16_t w, int16_t h, bool loop, size_t frames, uint8_t chroma_key)
      : _x(x), _y(y), _w(w), _h(h), _loop(loop), _frames(frames)
  {
    _has_chroma_key = true;
    _transparent_color = palette[chroma_key];
    init_bitmap(bitmap, palette, w * h * frames);
  }

  void init_bitmap(uint8_t *bitmap, uint16_t *palette, size_t size)
  {
    _bitmap = (uint16_t *)malloc(size * 2);
    uint16_t *p = _bitmap;
    while (size--)
    {
      *p++ = palette[*bitmap++];
    }
  }

  void h_scroll(int16_t v)
  {
    _x += v;
    if (_x < -(_w))
    {
      _x = _w;
    }
    else if (_x > _w)
    {
      _x = -(_w);
    }
  }

  void h_scroll(int16_t v, int16_t bound)
  {
    _x += v;
    if (_x < -(_w))
    {
      _x = bound;
    }
    else if (_x > bound)
    {
      _x = -(_w);
    }
  }

  void next_frame()
  {
    _curr_frame++;
    if (_curr_frame >= _frames)
    {
      _curr_frame = 0;
    }
  }

  void draw(Arduino_GFX *gfx)
  {
    if (_has_chroma_key)
    {
      gfx->draw16bitRGBBitmap(_x, _y, _bitmap + (_curr_frame * _w * _h), _transparent_color, _w, _h);
      if (_loop)
      {
        gfx->draw16bitRGBBitmap((_x < 0) ? (_x + _w) : (_x - _w), _y, _bitmap + (_curr_frame * _w * _h), _transparent_color, _w, _h);
      }
    }
    else
    {
      gfx->draw16bitRGBBitmap(_x, _y, _bitmap + (_curr_frame * _w * _h), _w, _h);
      if (_loop)
      {
        gfx->draw16bitRGBBitmap((_x < 0) ? (_x + _w) : (_x - _w), _y, _bitmap + (_curr_frame * _w * _h), _w, _h);
      }
    }
  }

private:
  int16_t _x;
  int16_t _y;
  uint16_t *_bitmap;
  int16_t _w;
  int16_t _h;
  bool _loop;
  uint16_t _transparent_color;
  bool _has_chroma_key = false;
  size_t _frames;
  size_t _curr_frame = 0;
};
