/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#pragma once

#include <docmodel/dllapi.h>
#include <tools/color.hxx>
#include <docmodel/theme/ThemeColor.hxx>
#include <com/sun/star/graphic/XGraphic.hpp>

namespace model
{
enum class ColorType
{
    Unused,
    RGB,
    CRGB,
    HSL,
    Scheme,
    Palette,
    System,
    Placeholder
};

enum class SystemColorType
{
    Unused,
    DarkShadow3D,
    Light3D,
    ActiveBorder,
    ActiveCaption,
    AppWorkspace,
    Background,
    ButtonFace,
    ButtonHighlight,
    ButtonShadow,
    ButtonText,
    CaptionText,
    GradientActiveCaption,
    GradientInactiveCaption,
    GrayText,
    Highlight,
    HighlightText,
    HotLight,
    InactiveBorder,
    InactiveCaption,
    InactiveCaptionText,
    InfoBack,
    InfoText,
    Menu,
    MenuBar,
    MenuHighlight,
    MenuText,
    ScrollBar,
    Window,
    WindowFrame,
    WindowText
};

struct DOCMODEL_DLLPUBLIC ColorDefinition
{
    ColorType meType = ColorType::Unused;

    sal_Int32 mnComponent1 = 0; // Red, Hue
    sal_Int32 mnComponent2 = 0; // Green, Saturation
    sal_Int32 mnComponent3 = 0; // Blue, Luminance
    sal_Int32 mnAlpha = 0; // Percentage

    SystemColorType meSystemColorType = SystemColorType::Unused;
    ::Color maLastColor;

    ThemeColorType meSchemeType = ThemeColorType::Unknown;
    std::vector<Transformation> maTransformations;

    void setCRGB(sal_Int32 nR, sal_Int32 nG, sal_Int32 nB)
    {
        mnComponent1 = nR;
        mnComponent2 = nG;
        mnComponent3 = nB;
        meType = ColorType::CRGB;
    }

    void setRGB(sal_Int32 nRGB)
    {
        ::Color aColor(ColorTransparency, nRGB);
        mnComponent1 = aColor.GetRed();
        mnComponent2 = aColor.GetGreen();
        mnComponent3 = aColor.GetBlue();
        meType = ColorType::RGB;
    }

    void setHSL(sal_Int32 nH, sal_Int32 nS, sal_Int32 nL)
    {
        mnComponent1 = nH;
        mnComponent2 = nS;
        mnComponent3 = nL;
        meType = ColorType::HSL;
    }

    void setSystemColor(SystemColorType eSystemColorType, sal_Int32 nRGB)
    {
        maLastColor = ::Color(ColorTransparency, nRGB);
        meSystemColorType = eSystemColorType;
        meType = ColorType::System;
    }

    void setSchemePlaceholder() { meType = ColorType::Placeholder; }

    void setSchemeColor(ThemeColorType eType)
    {
        meSchemeType = eType;
        meType = ColorType::Scheme;
    }
};

enum class FillType
{
    None,
    Solid,
    Gradient,
    Pattern,
    Blip
};

class DOCMODEL_DLLPUBLIC Fill
{
public:
    Fill(FillType eType)
        : meType(eType)
    {
    }

    FillType meType;
};

class DOCMODEL_DLLPUBLIC NoFill : public Fill
{
public:
    NoFill()
        : Fill(FillType::None)
    {
    }
};

class DOCMODEL_DLLPUBLIC SolidFill : public Fill
{
public:
    ColorDefinition maColorDefinition;

    SolidFill()
        : Fill(FillType::Solid)
    {
    }
};

class DOCMODEL_DLLPUBLIC GradientStop
{
public:
    double mfPosition = 0.0; // 0.0 - 1.0
    ColorDefinition maColor;
};

enum class GradientType
{
    Undefined,
    Linear,
    Circle,
    Rectangle,
    Shape,
};

struct DOCMODEL_DLLPUBLIC LinearGradientProperties
{
    sal_Int32 mnAngle = 0;
    bool mbScaled = false;
};

struct DOCMODEL_DLLPUBLIC RelativeRectangle
{
    sal_Int32 mnLeft = 0;
    sal_Int32 mnTop = 0;
    sal_Int32 mnRight = 0;
    sal_Int32 mnBottom = 0;
};

class DOCMODEL_DLLPUBLIC GradientFill : public Fill
{
public:
    bool mbRotateWithShape = false;
    GradientType meGradientType = GradientType::Undefined;
    std::vector<GradientStop> maGradientStops;
    LinearGradientProperties maLinearGradient;
    RelativeRectangle maFillToRectangle;
    RelativeRectangle maTileRectangle;

    GradientFill()
        : Fill(FillType::Gradient)
    {
    }
};

enum class PatternPreset
{
    Unused,
    Percent_5,
    Percent_10,
    Percent_20,
    Percent_25,
    Percent_30,
    Percent_40,
    Percent_50,
    Percent_60,
    Percent_70,
    Percent_75,
    Percent_80,
    Percent_90,
    Horizontal,
    Vertical,
    LightHorizontal,
    LightVertical,
    DarkHorizontal,
    DarkVertical,
    NarrowHorizontal,
    NarrowVertical,
    DashedHorizontal,
    DashedVertical,
    Cross,
    DownwardDiagonal,
    UpwardDiagonal,
    LightDownwardDiagonal,
    LightUpwardDiagonal,
    DarkDownwardDiagonal,
    DarkUpwardDiagonal,
    WideDownwardDiagonal,
    WideUpwardDiagonal,
    DashedDownwardDiagonal,
    DashedUpwardDiagonal,
    DiagonalCross,
    SmallCheckerBoard,
    LargeCheckerBoard,
    SmallGrid,
    LargeGrid,
    DottedGrid,
    SmallConfetti,
    LargeConfetti,
    HorizontalBrick,
    DiagonalBrick,
    SolidDiamond,
    OpenDiamond,
    DottedDiamond,
    Plaid,
    Sphere,
    Weave,
    Divot,
    Shingle,
    Wave,
    Trellis,
    ZigZag
};

class DOCMODEL_DLLPUBLIC PatternFill : public Fill
{
public:
    PatternPreset mePatternPreset = PatternPreset::Unused;
    ColorDefinition maForegroundColor;
    ColorDefinition maBackgroundColor;

    PatternFill()
        : Fill(FillType::Pattern)
    {
    }
};

enum class BitmapMode
{
    Unused,
    Tile,
    Stretch,
};

enum class FlipMode
{
    None,
    X,
    Y,
    XY
};

enum class RectangleAlignment
{
    Unset,
    TopLeft,
    Top,
    TopRight,
    Left,
    Center,
    Right,
    BottomLeft,
    Bottom,
    BottomRight
};

enum class ColorEffectType
{
    None,
    BiLevel,
    Grayscale,
    ColorChange
};

class DOCMODEL_DLLPUBLIC BlipFill : public Fill
{
public:
    bool mbRotateWithShape = false;
    RelativeRectangle maClipRectangle;
    RelativeRectangle maFillRectangle;
    BitmapMode meMode = BitmapMode::Unused;

    sal_Int32 mnTileOffsetX = 0;
    sal_Int32 mnTileOffsetY = 0;
    sal_Int32 mnTileScaleX = 0;
    sal_Int32 mnTileScaleY = 0;
    FlipMode meTileFlipMode = FlipMode::None;
    RectangleAlignment meTileAlignment = RectangleAlignment::TopLeft;

    css::uno::Reference<css::graphic::XGraphic> mxGraphic;

    ColorEffectType meColorEffectType = ColorEffectType::None;

    sal_Int32 mnBiLevelThreshold = 0;
    ColorDefinition maColorFrom;
    ColorDefinition maColorTo;
    bool mbUseAlpha = false;

    BlipFill()
        : Fill(FillType::Blip)
    {
    }
};

class DOCMODEL_DLLPUBLIC FillStyle
{
public:
    std::shared_ptr<Fill> mpFill;
};

enum class CapType
{
    Unset,
    Flat,
    Round,
    Square
};

enum class PenAlignmentType
{
    Unset,
    Center,
    Inset
};

enum class CompoundLineType
{
    Unset,
    Single,
    Double,
    ThickThin_Double,
    ThinThick_Double,
    Triple,
};

enum class PresetDashType
{
    Unset,
    Dash,
    DashDot,
    Dot,
    LargeDash,
    LargeDashDot,
    LargeDashDotDot,
    Solid,
    SystemDash,
    SystemDashDot,
    SystemDashDotDot,
    SystemDot,
};

enum class LineJoinType
{
    Unset,
    Round,
    Bevel,
    Miter,
};

struct DOCMODEL_DLLPUBLIC LineJoin
{
    LineJoinType meType = LineJoinType::Unset;
    sal_Int32 mnMiterLimit = 0; // Percentage
};

enum class LineEndType
{
    None,
    Triangle,
    Stealth,
    Diamond,
    Oval,
    Arrow
};

enum class LineEndWidth
{
    Unset,
    Small,
    Medium,
    Large
};

enum class LineEndLength
{
    Unset,
    Small,
    Medium,
    Large
};

struct DOCMODEL_DLLPUBLIC LineEnd
{
    LineEndType meType;
    LineEndWidth meWidth;
    LineEndLength meLength;
};

struct DOCMODEL_DLLPUBLIC DashStop
{
    sal_Int32 mnDashLength = 0;
    sal_Int32 mnStopLength = 0;
};

struct DOCMODEL_DLLPUBLIC LineDash
{
    PresetDashType mePresetType = PresetDashType::Unset;
    std::vector<DashStop> maCustomList;
};

class DOCMODEL_DLLPUBLIC LineStyle
{
public:
    sal_Int32 mnWidth;
    CapType meCapType;
    PenAlignmentType mePenAlignment;
    CompoundLineType meCompoundLineType;
    LineDash maLineDash;
    LineJoin maLineJoin;
    LineEnd maHeadEnd;
    LineEnd maTailEnd;

    FillStyle maLineFillStyle;
};

enum class EffectType
{
    Unset,
    OuterShadow,
    InnerShadow,
    Glow,
    SoftEdge,
    Reflection,
    Blur
};

class DOCMODEL_DLLPUBLIC Effect
{
public:
    EffectType meType = EffectType::Unset;
    sal_Int32 mnBlurRadius = 0;
    sal_Int32 mnRadius = 0;
    sal_Int32 mnDistance = 0;
    sal_Int32 mnDirection = 0;
    sal_Int32 mnScaleX = 100;
    sal_Int32 mnScaley = 100;
    sal_Int32 mnScewX = 0;
    sal_Int32 mnScewY = 0;
    RectangleAlignment meAlignment = RectangleAlignment::Bottom;
    bool mbRotateWithShape = true;
    ColorDefinition maColor;
    double mnEndAlpha = 100.0;
    double mnEndPosition = 0.0;
    double mnStartAlpha = 0.0;
    double mnStartPosition = 100.0;
    sal_Int32 mnFadeDirection = 0;
    bool mbGrow = false;
};

class DOCMODEL_DLLPUBLIC EffectStyle
{
public:
    std::vector<Effect> maEffectList;
};

class DOCMODEL_DLLPUBLIC FormatScheme
{
private:
    OUString maName;
    std::vector<FillStyle> maFillStyleList;
    std::vector<LineStyle> maLineStyleList;
    std::vector<EffectStyle> maEffectStyleList;
    std::vector<FillStyle> maBackgroundFillStyleList;

public:
    FormatScheme() = default;

    FormatScheme(OUString const& rName)
        : maName(rName)
    {
    }

    const OUString& getName() const { return maName; }

    std::vector<LineStyle> const& getLineStyleList() const { return maLineStyleList; }

    LineStyle* addLineStyle()
    {
        if (maLineStyleList.size() > 3)
            return nullptr;
        auto& rLineStyle = maLineStyleList.emplace_back();
        return &rLineStyle;
    }

    std::vector<EffectStyle> const& getEffectStyleList() const { return maEffectStyleList; }

    EffectStyle* addEffectStyle()
    {
        if (maEffectStyleList.size() > 3)
            return nullptr;
        auto& rEffectStyle = maEffectStyleList.emplace_back();
        return &rEffectStyle;
    }

    std::vector<FillStyle> const& getFillStyleList() const { return maFillStyleList; }

    FillStyle* addFillStyle()
    {
        if (maFillStyleList.size() > 3)
            return nullptr;
        auto& rFillStyle = maFillStyleList.emplace_back();
        return &rFillStyle;
    }

    std::vector<FillStyle> const& getBackgroundFillStyleList() const
    {
        return maBackgroundFillStyleList;
    }

    FillStyle* addBackgroundFillStyle()
    {
        if (maBackgroundFillStyleList.size() > 3)
            return nullptr;
        auto& rBackgroundFillStyle = maBackgroundFillStyleList.emplace_back();
        return &rBackgroundFillStyle;
    }
};

} // end of namespace svx

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
