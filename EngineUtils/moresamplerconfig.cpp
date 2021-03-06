﻿#include "moresamplerconfig.h"

MoresamplerConfig::MoresamplerConfig(QString &configString):configString(configString)
{
    processString();
}

MoresamplerConfig::~MoresamplerConfig()
{
    if (editMode)
        delete editMode;
}

QString MoresamplerConfig::getConfigString() const
{
    return configString;
}

void MoresamplerConfig::setConfigString(const QString &value)
{
    configString = value;
    processString();
}

MoresamplerConfig::ConfigType MoresamplerConfig::getType() const
{
    return type;
}

QString MoresamplerConfig::getTypeString(QString name)
{
    return getTypeString(getType(name));
}

void MoresamplerConfig::processString()
{
    configString = configString.trimmed();
    if (configString.isEmpty())
    {
        type = ConfigType::Blank;
        nameString = QCoreApplication::translate("MoresamplerConfig", "（空行）");
        valueString = QCoreApplication::translate("MoresamplerConfig", "（空行）");
        return;
    }
    if (configString.at(0) == "#")
    {
        decoration.comment = true;
        nameString = QCoreApplication::translate("MoresamplerConfig", "（注释）");
        valueString = configString.mid(1);
        type = ConfigType::Comment;
        editMode = getEditMode(QString());
        value = editMode->toVariantValueFromString(valueString);
        return;
    }
    if (configString.at(0) == "*")
    {
        configString.remove(0,1);
        decoration.override = true;
    }
    auto splitted = configString.split(" ",QString::SplitBehavior::SkipEmptyParts);
    nameString = splitted.at(0).trimmed();
    type = getType(nameString);
    if (splitted.count() >= 2)
        valueString = splitted.at(1).trimmed();
    editMode = getEditMode(nameString);
    if (valueString.trimmed().isEmpty()){
        valueString = editMode->toStringFromVariantValue(value);
    }
    value = editMode->toVariantValueFromString(valueString);
    if (valueString.trimmed().isEmpty())
        valueString = editMode->toStringFromVariantValue(value);
}

MoresamplerConfig::ConfigType MoresamplerConfig::getType(const QString& configName)
{
    if (configName == "output-sampling-rate" || configName == "output-bit-depth" || configName == "resampler-compatibility")
        return ConfigType::Output;
    if (configName == "synthesis-utau-style-normalization" || configName == "synthesis-loudness-preservation" || configName == "synthesis-duration-extension-method")
        return ConfigType::Synthesis;
    if (configName == "multithread-synthesis" || configName == "auto-update-llsm-mrq" || configName == "dump-log-file")
        return ConfigType::Misc;
    if (configName == "analysis-f0-range-from-path" || configName == "analysis-biased-f0-estimation" || configName == "analysis-f0-min" || configName == "analysis-f0-max" || configName == "load-frq" || configName == "analysis-anti-distortion" || configName == "analysis-noise-reduction" || configName == "analysis-suppress-subharmonics")
        return ConfigType::Analysis;
    if (configName.startsWith("meta-flag-"))
        return ConfigType::MetaFlag;
    return ConfigType::Unknown;
}

QString MoresamplerConfig::getTypeString(const ConfigType &type)
{
    switch (type) {
    case ConfigType::Analysis:
        return QCoreApplication::translate("MoresamplerConfig", "分析配置");
    case ConfigType::MetaFlag:
        return QCoreApplication::translate("MoresamplerConfig", "元标记");
    case ConfigType::Misc:
        return QCoreApplication::translate("MoresamplerConfig", "杂项");
    case ConfigType::Output:
        return QCoreApplication::translate("MoresamplerConfig", "输出配置");
    case ConfigType::Synthesis:
        return QCoreApplication::translate("MoresamplerConfig", "合成配置");
    case ConfigType::Unknown:
        return QCoreApplication::translate("MoresamplerConfig", "未知");
    case ConfigType::Comment:
        return QCoreApplication::translate("MoresamplerConfig", "注释");
    case ConfigType::Blank:
        return QCoreApplication::translate("MoresamplerConfig", "空行");
    }
    return QCoreApplication::translate("MoresamplerConfig", "未知");
}

QString MoresamplerConfig::getEntryHelp(const QString &configName)
{
    if (configName.startsWith("meta-flag-") || configName == QCoreApplication::translate("MoresamplerConfig", "（元标记）"))
        return QCoreApplication::translate("MoresamplerConfig", "元标记提供了一种快捷的方式让您多个flag合并为一个。元标记在全局配置文件中被定义,并通过 M+数字 (例如 M1, M2, M3) 的形式使用。通过在元标记之后放置点和数字，对应的 flag 的有效数值将按照数字缩放（以百分比表示，只要缩放的结果仍然在每个标志的允许范围内）。");
    return qApp->translate("MoresamplerHelp",entryHelps.value(configName).toStdString().data());
}

QString MoresamplerConfig::toString() const
{
    if (type == ConfigType::Blank)
        return QString();
    auto result = nameString + " " + valueString;
    if (decoration.override)
        result.insert(0,"*");
    if (decoration.comment)
        result.insert(0,"#");
    return result;
}

bool MoresamplerConfig::isValidValue() const
{
    LeafLogger::LogMessage(QString("Moresampler配置项%1的值为%2，有效性为%3").arg(nameString).arg(value.toString()).arg(editMode->isValidValue(value)));
    return editMode->isValidValue(value);
}

MoresamplerConfig::EditMode *MoresamplerConfig::getEditMode(const QString &configName)
{
    if (configName == "output-sampling-rate")
        return new PositiveIntegerEditMode();
    else if (configName == "output-bit-depth")
        return new ChoicesEditMode({"8","16","24","32"});
    else if (configName == "resampler-compatibility")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "synthesis-utau-style-normalization")
        return new ChoicesEditMode({"full","voiced","off"});
    else if (configName == "synthesis-loudness-preservation")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "synthesis-duration-extension-method")
        return new ChoicesEditMode({"auto","stretch","loop"});
    else if (configName == "multithread-synthesis")
        return new ChoicesEditMode({"full","on","off"});
    else if (configName == "auto-update-llsm-mrq")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "dump-log-file")
        return new StringEditMode();
    else if (configName == "analysis-f0-range-from-path")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "analysis-biased-f0-estimation")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "analysis-f0-min")
        return new PositiveDoubleEditMode();
    else if (configName == "analysis-f0-max")
        return new PositiveDoubleEditMode();
    else if (configName == "load-frq")
        return new ChoicesEditMode({"strict","on","off"});
    else if (configName == "analysis-anti-distortion")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "analysis-noise-reduction")
        return new ChoicesEditMode({"on","off"});
    else if (configName == "analysis-suppress-subharmonics")
        return new ChoicesEditMode({"on","off"});
    else
        return new StringEditMode();
}

void MoresamplerConfig::setValue(const QVariant &value)
{
    if (editMode->isValidValue(value))
    {
        this->value = value;
        valueString = editMode->toStringFromVariantValue(value);
    }
    else
    {
        throw ValueNotValidException();
    }
}

QStringList MoresamplerConfig::getAllEntrysName()
{
    return entryHelps.keys();
}

QVariant MoresamplerConfig::getValue() const
{
    return value;
}


QString MoresamplerConfig::getNameString() const
{
    return nameString;
}

QString MoresamplerConfig::getEntryHelp() const
{
    return getEntryHelp(getNameString());
}

MoresamplerConfig::EditMode *MoresamplerConfig::getEditMode()
{
    return editMode;
}

QString MoresamplerConfig::getValueString() const
{
    return valueString;
}

const QHash<QString,QString> MoresamplerConfig::entryHelps{
    {"output-sampling-rate",QT_TRANSLATE_NOOP("MoresamplerHelp", "输出的 .wav 文件的采样频率")},
    {"output-bit-depth",QT_TRANSLATE_NOOP("MoresamplerHelp", "输出的 .wav 文件的位深")},
    {"resampler-compatibility",QT_TRANSLATE_NOOP("MoresamplerHelp", "开启时, 在resampler模式下生成 .wav 文件 （用于与其他wavtool兼容）。此选项将减慢合成。")},
    {"synthesis-utau-style-normalization",QT_TRANSLATE_NOOP("MoresamplerHelp", "对每个音符应用自适应增益，使得当音量为100％时，合成波形的峰值达到最大幅度的一半。full: 对浊音和清音均进行增益； voiced: 只增益浊音；off: 不改变音量。")},
    {"synthesis-loudness-preservation",QT_TRANSLATE_NOOP("MoresamplerHelp", "基于心理声学响度测量，在修改后使可感知响度保持。")},
    {"synthesis-duration-extension-method",QT_TRANSLATE_NOOP("MoresamplerHelp", "决定Moresampler如何使音符时间变长。auto: 根据源时长和目标时长自动决定是使用拉伸方式还是循环方式。可使用 e 和 Me flag 来覆盖这一行为。")},
    {"multithread-synthesis",QT_TRANSLATE_NOOP("MoresamplerHelp", "on: wavtool模式下的合成将多线程运行（将会更快）。full: resampler模式也将在多线程运行。“多线程”意味着将多个线程包装于一个进程中，与同一时间运行多个Moresampler的“多进程”有着本质不同。注意，该特性不被 moresampler-legacy.exe支持。")},
    {"auto-update-llsm-mrq",QT_TRANSLATE_NOOP("MoresamplerHelp", "检查.wav 文件与对应.llsm 文件和mrq数据项的最后修改时间。如果.wav文件比.llsm要新，则进行重分析。若.wav文件也比mrq数据项要新，则在重分析以生成.llsm文件前进行音高重估计。此特性或许对声库开发者有帮助。")},
    {"dump-log-file",QT_TRANSLATE_NOOP("MoresamplerHelp", "在指定文件路径中输出Debug信息。")},
    {"analysis-f0-range-from-path",QT_TRANSLATE_NOOP("MoresamplerHelp", "从文件夹名推测音高。例如，如果给定的样本位于一个名为“C_D4”的文件夹下，Moresampler将在接近D4的范围（294 Hz左右）内进行音高分析。")},
    {"analysis-biased-f0-estimation",QT_TRANSLATE_NOOP("MoresamplerHelp", "在接合音高与浊音行为估计期间增大浊音化概率，随后进行浊音音高修正；旨在提高特异度和降低灵敏度；适用于嘈杂/粗糙的语音，但会降低干净/流畅语音的质量。")},
    {"analysis-f0-min",QT_TRANSLATE_NOOP("MoresamplerHelp", "音高的下界(以Hz为单位)。")},
    {"analysis-f0-max",QT_TRANSLATE_NOOP("MoresamplerHelp", "音高的上界(以Hz为单位)。")},
    {"load-frq",QT_TRANSLATE_NOOP("MoresamplerHelp", "strict : 当desc.mrq不可用时，从.frq 文件读取音高数据。on : 使用 .frq 文件的音高数据来修正Moresampler自己对音高的猜测。不管该选项是否开启，结果都会保存至desc.mrq。注意：除非被仔细修正，.frq文件提供的音高数据通常无法足够可靠地使Moresampler运作。")},
    {"analysis-anti-distortion",QT_TRANSLATE_NOOP("MoresamplerHelp", "on : Moresampler将自动修复由噪声失真或低音量（量化误差）引起的分析错误。这些错误可能造成音高变换后的“尖锐”与“粗糙”的声音。然而此特性（理论上）可能导致轻微模糊。")},
    {"analysis-noise-reduction",QT_TRANSLATE_NOOP("MoresamplerHelp", "在从.wav文件分析LLSM时自动降噪。更适用于长录音。")},
    {"analysis-suppress-subharmonics",QT_TRANSLATE_NOOP("MoresamplerHelp", "在分析时自动移除输入中存在的次谐波。对尖亮的声音可能有帮助，但会导致呼吸音的质量降低。")},
};

MoresamplerConfig::EditMode::NotChoicesException::NotChoicesException() : std::runtime_error("The value of this config is not a \"Choices\".")
{

}

MoresamplerConfig::EditMode::EditMode(MoresamplerConfig::EditMode::ValueType valueType, QStringList choices) : choices(choices),valueType(valueType)
{

}


MoresamplerConfig::EditMode::ValueType MoresamplerConfig::EditMode::getValueType() const
{
    return valueType;
}

QStringList MoresamplerConfig::EditMode::getChoices() const
{
    if (valueType == ValueType::Choices)
        return choices;
    else
        throw NotChoicesException();
}

MoresamplerConfig::EditMode::~EditMode()
{

}

MoresamplerConfig::DoubleEditMode::DoubleEditMode() : EditMode(Double)
{

}

bool MoresamplerConfig::DoubleEditMode::isValidValue(QVariant value) const
{
    return value.isValid() && value.type() == QVariant::Type::Double;
}

QVariant MoresamplerConfig::DoubleEditMode::toVariantValueFromString(QString valueString) const
{
    bool ok = false;
    auto value = valueString.toDouble(&ok);
    if (ok && isValidValue(value))
        return value;
    else
        return 0.0;
}

QString MoresamplerConfig::DoubleEditMode::toStringFromVariantValue(QVariant value) const
{
    if (isValidValue(value))
        return QString("%1").arg(value.toDouble());
    else
        return QString("%1").arg(0.0);
}

MoresamplerConfig::PositiveDoubleEditMode::PositiveDoubleEditMode() : DoubleEditMode ()
{

}

bool MoresamplerConfig::PositiveDoubleEditMode::isValidValue(QVariant value) const
{
    return DoubleEditMode::isValidValue(value) && value.toDouble() > 0;
}

MoresamplerConfig::IntegerEditMode::IntegerEditMode() : EditMode(ValueType::Integer)
{

}

bool MoresamplerConfig::IntegerEditMode::isValidValue(QVariant value) const
{
    return value.isValid() && value.type() == QVariant::Type::Int;
}

QVariant MoresamplerConfig::IntegerEditMode::toVariantValueFromString(QString valueString) const
{
    bool ok = false;
    auto value = valueString.toInt(&ok);
    if (ok && isValidValue(value))
        return value;
    else
        return 0;
}

QString MoresamplerConfig::IntegerEditMode::toStringFromVariantValue(QVariant value) const
{
    if (isValidValue(value))
        return QString("%1").arg(value.toInt());
    else
        return QString("%1").arg(0);
}

MoresamplerConfig::PositiveIntegerEditMode::PositiveIntegerEditMode() : IntegerEditMode ()
{

}

bool MoresamplerConfig::PositiveIntegerEditMode::isValidValue(QVariant value) const
{
    return IntegerEditMode::isValidValue(value) && value.toInt() > 0;
}

MoresamplerConfig::StringEditMode::StringEditMode() : EditMode (ValueType::String)
{

}

bool MoresamplerConfig::StringEditMode::isValidValue(QVariant value) const
{
    return value.isValid() && value.type() == QVariant::Type::String;
}

QVariant MoresamplerConfig::StringEditMode::toVariantValueFromString(QString valueString) const
{
    if (isValidValue(valueString))
        return valueString;
    else
        return QVariant();
}

QString MoresamplerConfig::StringEditMode::toStringFromVariantValue(QVariant value) const
{
    if (isValidValue(value))
        return value.toString();
    else
        return QString();
}

MoresamplerConfig::ChoicesEditMode::ChoicesEditMode(QStringList choices) : EditMode (ValueType::Choices,choices)
{

}

bool MoresamplerConfig::ChoicesEditMode::isValidValue(QVariant value) const
{
    return value.isValid() && value.type() == QVariant::Type::String && choices.contains(value.toString());
}

QVariant MoresamplerConfig::ChoicesEditMode::toVariantValueFromString(QString valueString) const
{
    if (isValidValue(valueString))
        return valueString;
    else
        return choices.at(0);
}

QString MoresamplerConfig::ChoicesEditMode::toStringFromVariantValue(QVariant value) const
{
    if (value.type() == QVariant::String && isValidValue(value))
        return value.toString();
    else
        return choices.at(0);
}

MoresamplerConfig::ValueNotValidException::ValueNotValidException() : std::runtime_error("The given value is not valid for this config.")
{

}
