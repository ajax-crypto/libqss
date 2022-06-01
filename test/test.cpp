#include <QCoreApplication>

#include "qssdocument.h"

#define RESULTV(A, B, V) LOG(A << " should be: " << #V << " | Test pass status: " << (B == V));
#define RESULTSTR(A, B, V) LOG(A << " should be: " << V << " | Test pass status: " << (B == V));
 
void TestQSSParts()
{
    LOG("\n\nBasic QSS Parsing...");
    QString test = "select#id.class1.class2[param1=\"val1\"][param2=\"val2\"]::sub-ctrl:psuedo "
        "{ prop1: val1 val2; prop2: val3 val4; }";
    qss::Fragment fragment{ test };
    const auto& selector = fragment.selector().front();
    const auto& block = fragment.block();
    
    RESULTV("Selector count", fragment.selector().fragmentCount(), 1);
    RESULTSTR("Selector ID", selector.id(), "id");
    RESULTSTR("Selector Name", selector.name(), "select");
    RESULTSTR("Selector Subcontrol", selector.subControl(), "sub-ctrl");
    RESULTSTR("Selector Pseudo class", selector.psuedoClass(), "psuedo");
    RESULTV("Selector class count", selector.classCount(), 2);
    RESULTV("Selector param count", selector.paramCount(), 2);
    RESULTV("Block property count", block.size(), 2);
}

void TestQSSText()
{
    LOG("\n\nParsing 3 block of QSS...");
    QString test = "aaa, xxx { bb: cc; } dd{ aa: \"{}\"; cc : dd; }";
    qss::Document qss{test};
    qss += "xxx { yy: zz; }";

    RESULTV("Total Fragments", qss.totalFragments(), 3);
    RESULTV("Total selector fragment for 1st block", qss[0].selector().fragmentCount(), 2);

    qss.toggleFragment(0);
    RESULTV("Total active fragments", qss.totalActiveFragments(), 2);

    qss.enableFragment(0);
    RESULTV("Total active fragments", qss.totalActiveFragments(), 3);

    LOG("Adding block with existing selector...");
    qss += "xxx { aa: bb; }";
    RESULTV("Block count", qss.totalFragments(), 3);
    RESULTV("Property count for last block", qss.back().block().size(), 2);

    qss.removeFragment(1);
    RESULTV("Removing one from block, property count", qss.totalFragments(), 2);
}

void TestQSSParse()
{
    LOG("\n\nComparing parsed and generated QSS...");
    QString test1 = "#parent name[prop=\"val ff \"][aaa=\"bb b\"]::sub-control:hover  ~ QLabel#child { "
            "   background-color: white;"
            "   border: 1px solid black;"
            "}";

    qss::Fragment fragment{test1};
    fragment.addParam("color", "red").enableParam("background-color", false);

    qss::Fragment manual;
    manual.selector()
            .append("#parent", qss::SelectorElement::PARENT)
            .addDescendant(qss::SelectorElement().select("name").on("prop", "val ff ").on("aaa", "bb b").sub("sub-control").when("hover"))
            .addSibling("QLabel#child");

    manual.addParam("border", "1px solid black");
    manual.addParam("color", "red");
    LOG("Passed: " << (QString::compare(fragment.toString(), manual.toString()) == 0));
}

int main(int argc, char *argv[])
{
    using qss::operator<<;
    QCoreApplication a(argc, argv);
    LOG(std::boolalpha);

    try
    {
        TestQSSParts();
        TestQSSText();
        TestQSSParse();
    }
    catch (const qss::Exception& except)
    {
        std::cout << except.what();
    }

    return 0;
}


