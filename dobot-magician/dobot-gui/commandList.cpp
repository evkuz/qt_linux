#include "commandList.h"
#include <tuple>
#include <iostream>
#include <vector>
#include <string>
#include <QList>


using namespace std;

QList<QString> commands = {"GetPose", "Home"};
QList<char> commandIDs = {10, 31};
void setupCommandList(){
    //tuple<Qstring, int>

    vector<tuple<string, int>> data;
    for(int i = 0; i < commandIDs.size(); i++){
        data.emplace_back(commands.at(i), commandIDs.at(i));
    }


}
