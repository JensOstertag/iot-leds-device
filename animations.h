struct RgbColor {
  const int r;
  const int g;
  const int b;

  RgbColor(unsigned const int r, unsigned const int g, unsigned const int b) : r(r), g(g), b(b) {}
};

struct Color {
  const String hexColor;
  const RgbColor rgbColor;

  Color(const String hexColor, const RgbColor rgbColor) : hexColor(hexColor), rgbColor(rgbColor) {}
};

struct Animation {
  const int id;
  const String animationType;
  const Color* colors;
  const int durationPerColor;

  float animationProgress = 0;

  Animation(const int id, const String animationType, const Color colors[], const int durationPerColor) : id(id), animationType(animationType), colors(colors), durationPerColor(durationPerColor) {}
};
