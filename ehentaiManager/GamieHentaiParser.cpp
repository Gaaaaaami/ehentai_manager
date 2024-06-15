#include "GamieHentaiParser.h"

#include <QDebug>


GamieHentaiParser::GamieHentaiParser(QString msg){
    _msg = msg;
}
GamieHentaiParser::~GamieHentaiParser(){}

QString GamieHentaiParser::GetTag(QString tag, QString classname){
    auto lst = GetAllTag(tag);
    QString         t = "";
    for(auto item : lst)
    {
        if(classname == GetClass(item)){
           t = item;
        }
    }
    if(t.size() == 0)
        return "";
    return GetTagEnd(t);
}

QString GamieHentaiParser::GetTagById(QString tag, QString id){
    auto lst = GetAllTag(tag);
    QString         t = "";
    for(auto item : lst)
    {
        QString tag_id = GetID(item);
        tag_id = ToNormalURL(tag_id);
        if(id == tag_id){
           t = item;
        }
    }
    if(t.size() == 0)
        return "";
    return GetTagEnd(t);
}

QString GamieHentaiParser::GetContent(QString tag){
    QString final = "";
    int index = _msg.indexOf(tag);
    if(index < 0)
        return final;
    QString tagname = "</"+GetTagName(tag)+">";
    int end_index = _msg.indexOf(tagname,index);
    if(end_index < 0)
        return final;

    for(int i =index+tag.length();i < end_index;i++){
        final+=_msg.at(i);
    }
    return final;
}

QString GamieHentaiParser::GetAttribute(QString tag, QString attname)
{
    attname += "=";
    int index = tag.indexOf(attname);
    if( index < 0)
        return "";
    index+=attname.length();
    unsigned char c = 0;
    QString final = "";


    while(index < tag.length() && index >= 0){
        final += tag.at(index);
        if(tag.at(index) == '"' && c == 1)
            break;
        else if(c == 0)
            c++;

        index++;
    }
    return final;
}
QString GamieHentaiParser::GetTag(QString tag)
{
    auto lst = GetAllTag(tag);
    if(lst.size() == 0)
        return "";
    if(tag == "img")
        return lst.at(0);
    return GetTagEnd(lst.at(0));
}
QStringList GamieHentaiParser::GetAllTag(QString tag){
    QStringList final;
    QString search_tag = "<" + tag;
    int index = 0,lastest_tag_length = 0;
    while(index > -1){
        index = _msg.indexOf(search_tag,index+lastest_tag_length);
        if( index > -1){
            QString t = GetEnd(index);
            lastest_tag_length = t.length();
            final.push_back(t);
        }else
            break;
    }
    return final;
}

QStringList GamieHentaiParser::GetAllTrTag(){
    QStringList final;
    int index = 0,lastest_tag_length = 0;
    while(index > -1){
        index = _msg.indexOf("<tr",index+lastest_tag_length);
        if( index > -1){
            QString t = GetEnd(index);

            QString msg = _msg;
            int end_index = _msg.indexOf("</tr>",index+lastest_tag_length);

            if(index > -1 && end_index > -1){
                QString msg = _msg;
                msg.remove(end_index+strlen("</tr>"),msg.length()-end_index-strlen("</tr>"));
                msg.remove(0,index);
                final.push_back(msg);
            }

            lastest_tag_length = t.length();
        }else
            break;
    }
    return final;
}

QStringList GamieHentaiParser::GetAlltdTagInfo(){
    QStringList final;
    int index = 0,lastest_tag_length = 0;
    while(index > -1){
        index = _msg.indexOf("<td ",index+lastest_tag_length);
        if( index > -1){
            QString t = GetEnd(index);
            QString s = "";
            int end_index = _msg.indexOf("</td>",index);

            for(int i = index ;i < end_index+strlen("</td>"); i++)
            {
                s += _msg.at(i);
            }

            final.push_back(s);
            lastest_tag_length = t.length();
        }else
            break;
    }
    return final;
}

QString GamieHentaiParser::GetTagName(QString tag){
    QString final = "";
    int index = 1;
    while(index < tag.size()){
        if(tag.at(index) == " ")
            break;
        final += tag.at(index);
        index++;
    }

    return final;
}
QString GamieHentaiParser::GetTagEnd(QString tag, int start ){
    QString n = "</" + GetTagName(tag) + ">";
    int index = _msg.indexOf(tag);


    int end_index = _msg.indexOf(n, index);
    if(start != -1){
        index =start;
        end_index = _msg.indexOf(n, start);



    }


    if(index > -1 && end_index > -1){
        QString msg = _msg;
        msg.remove(end_index+n.size(),msg.length()-end_index-n.size());
        msg.remove(0,index);
        return msg;
    }else
        return "";
}
QString GamieHentaiParser::GetEnd(int start){
    QString final = "";
    int index  = start;
    while(index < _msg.length() && start >= 0){
        final +=  _msg[index];
        if(_msg[index] == '>')
           break;
        index++;
    }
    return final;
}
QString GamieHentaiParser::GetClass(QString obj){
    return GetAttribute(obj, "class");
}
QString GamieHentaiParser::GetID(QString obj){
    return GetAttribute(obj, "id");
}
QVector<GamieHentaiParser::steHentaiItemInfo> GamieHentaiParser::GetMainPageIndexList(){
    QVector<GamieHentaiParser::steHentaiItemInfo> final;
    GamieHentaiParser parser(GetTag("table" ,"\"itg gltc\""));
    auto tr = parser.GetAllTrTag();
    int ind = 1;
    for(auto &item : tr){
            GamieHentaiParser parsers(item);
            auto td = parsers.GetAlltdTagInfo();
            if(td.size() < 4){
               continue;
            }

            {
                QString base= td.at(2);
                GamieHentaiParser parser(base);
                auto a = parser.GetAllTag("a");
                for(auto item : a){

                    QString t = parser.GetTagEnd(item);
                    GamieHentaiParser parser(t);
                    QStringList div = parser.GetAllTag("div");
                    steHentaiItemInfo info;
                    info.title = parser.GetContent(div.at(0));
                    info.herf= parser.GetAttribute(item, "href");
                    info.index = ind;
                    final.push_back(info);
                }
            }
            ind++;
    }
    return final;
}

QVector<GamieHentaiParser::steHentaiItemInfo> GamieHentaiParser::GetImageList(){
    QVector<GamieHentaiParser::steHentaiItemInfo> final;
    const char *search_obj = "<div class=\"gdtm\"";
    int index = 0;
    while(index > -1){

        index = _msg.indexOf(search_obj, index);
        if(index < 0)
           break;
        QString content = GetTagEnd(GetEnd(index),index);
        GamieHentaiParser parser(content);
        auto list = parser.GetAllTag("a");

        for(auto item : list){
            GamieHentaiParser::steHentaiItemInfo info;
            info.herf = ToNormalURL( GetAttribute(item,"href"));
            final.push_back(info);
        }
        index += strlen(search_obj);
    }
    return final;
}
QVector<GamieHentaiParser::steHentaiItemInfo> GamieHentaiParser::GetImageList_PageHref(){
    QVector<GamieHentaiParser::steHentaiItemInfo> final;
    auto page_index_count = GetPageIndexToArray();
    int page_count = 0;
    if(!page_index_count.isEmpty())
        page_count = (page_index_count.end()-1)->page_index;
    page_index_count[0].herf = ToNormalURL(page_index_count[0].herf);
    for(int i = 0; i < page_count; i++){
        GamieHentaiParser::steHentaiItemInfo info;
        info.herf = page_index_count[0].herf + "/?p=" + QString::number(i);
        info.page_index = i;
        final.push_back(info);
    }
    return  final;
}

QVector<GamieHentaiParser::steHentaiItemInfo> GamieHentaiParser::GetPageIndexToArray(){
    GamieHentaiParser parser(GetTag("table" ,"\"ptt\""));
    auto tr = parser.GetAllTrTag();

    int ind = 1;
    QVector<steHentaiItemInfo> final;

    for(auto &item : tr){
        GamieHentaiParser parsers(item);
        auto td = parsers.GetAlltdTagInfo();


        for(auto &item : td)
        {
            GamieHentaiParser parsers(item);
            QStringList a= parsers.GetAllTag("a");
            if(a.length()!=0)
            {
               bool ret = false;
               QString content = parsers.GetContent(a.at(0));
               unsigned int d = content.toUInt(&ret);
               if(ret){

                   steHentaiItemInfo info;
                   if(!a.isEmpty())
                       info.herf = GetAttribute(a.at(0),"href");
                   info.page_index = d;
                   final.push_back(info);

               }

            }
        }
        ind++;
    }
    return final;
}

QString GamieHentaiParser::GetImageDownloadHref(){
    QString final = "";
    QString content = GetTagById("div", "i3");
    GamieHentaiParser parser(content);
    content = parser.GetTag("img");
    QString src = parser.GetAttribute(content, "src");
    final = ToNormalURL( src);





    return final;
}

QString GamieHentaiParser::ToNormalURL(QString addr){
    addr.remove( addr.length()-1,1);
    addr.remove( 0,1);
    return addr;
}




