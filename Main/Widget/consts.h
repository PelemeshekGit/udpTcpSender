#ifndef CONSTS_H
#define CONSTS_H

namespace Widget {

class Consts {
public:
    /// Максимально число слов
    static const int COUNTSAYS16 = 1024;

    static const int GEOMETRY_AX_LABEL = 2;
    static const int GEOMETRY_AY_LABEL = 20;
    static const int GEOMETRY_WIDTH_LABEL = 50;
    static const int GEOMETRY_HEIGHT_LABEL = 18;
    static const int GEOMETRY_AX_LINEEDIT = 50;
    static const int GEOMETRY_AY_LINEEDIT = 20;
    static const int GEOMETRY_WIDTH_LINEEDIT = 50;
    static const int GEOMETRY_HEIGHT_LINEEDIT = 18;
    static const int MAXLENGTH_SAY16 = 4;
    static const int BASEDATAHEX = 16;
    static const int GEOMETRY_WIDTH_CONTENTS = 120;
    static const int GEOMETRY_AY_CONTENTS = 40;
    static const int GEOMETRY_HEIGHT_LINE = 20;
    static const int GEOMETRY_WIDTH_LISTSAY16 = 450;

};

enum class STATUS_INDICATOR {
    Ok,
    Error,
    Current,
    None
};

}
#endif // CONSTS_H
