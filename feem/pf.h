#ifndef PF_H
#define PF_H
//2018-02-07
//by Poofee
//该类定义一些枚举

class PF{
public:
    enum ActionType{
        ActionNone,
        ActionDefault,

        ActionFlieNew,

    };

    enum Unit{
        None,
        Millimeter,
        Centimeter,
    };
    enum ZoomDirection {
        In, Out
    };
    enum RedrawMethod {
        RedrawNone = 0,
        RedrawGrid = 1,
        RedrawOverlay = 2,
        RedrawDrawing = 4,
        RedrawAll = 0xffff
    };

};
#endif // PF_H
