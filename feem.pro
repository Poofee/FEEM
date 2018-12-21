TEMPLATE = subdirs

SUBDIRS += \
    ./qtdock/qtdock.pro \
    ./QtGuider/QtGuider.pro \
    ./qtribbon/src/shared/ribbon \
    ./qtribbon/src/ribbondsgn \
    ./qtribbon/ribbonsample/ribbonsample.pro \
    ./feem/feem.pro \

TRANSLATIONS = $$PWD/feem/res/translations/feem_en.ts \
                $$PWD/feem/res/translations/feem_zh.ts \

CONFIG += ordered
