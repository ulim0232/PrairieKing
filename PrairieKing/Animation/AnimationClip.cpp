#include "stdafx.h"
#include "rapidcsv.h"

bool AnimationClip::LoadFromFile(const string& path)
{
    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1)); //0,0 이면 모든 행 읽어옴
    id = doc.GetCell<string>(0, 1); // 0행 1열
    fps = doc.GetCell<int>(1, 1);
    loopType = (AnimationLoopTypes)doc.GetCell<int>(2, 1);
   
    for (int i = 4; i<doc.GetRowCount(); i++)
    {
        auto rows = doc.GetRow<string>(i);
        frames.push_back({ rows[0], {stoi(rows[1]), stoi(rows[2]), stoi(rows[3]), stoi(rows[4]) } });
    }
    return true;
}
