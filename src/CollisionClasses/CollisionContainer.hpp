#pragma once

#include <vector>

#include "BaseCollision.hpp"

namespace Game::Collision {
    class CollisionContainer {

        // TODO: implement sorting bound boxes into blockmaps
        // so we don't have to check againstevery single bounding box
        std::vector<Game::Collision::BaseCollision*> Collisions;

        /*
        struct BlockAxisZ {
            //BlockAxisZ *Next, *Last;
            int AxisIndex;
            std::vector<Game::Collision::BaseCollision*> OverlappingCollisions;
        };
        struct BlockAxisY {
            //BlockAxisY *Next, *Last;
            int AxisIndex;
            std::vector<BlockAxisZ> ZAxis;
        };
        struct BlockAxisX {
            //BlockAxisX *Next, *Last;
            int AxisIndex;
            std::vector<BlockAxisY> YAxis;
            //BlockAxisY* YAxis;
        };
       
        std::vector<BlockAxisX> BlockmapStart;
        constexpr static const int BlockSize = 24;

        inline void FreeAxis() {
            BlockAxisX* FreeBlock;
            BlockAxisZ* ZBlock;
            BlockAxisY* YBlock;
            BlockAxisX* XBlock = this->Blockmap.Last;
            while (XBlock != NULL) {
                YBlock = XBlock->YAxis;
                while (YBlock != NULL) {
                    ZBlock = YBlock; //while (ZBlock != NULL) {}
                }
                FreeBlock = XBlock;
                XBlock = XBlock->Last;
                delete FreeBlock;
            }
            XBlock = this->Blockmap.Next;
            while (XBlock != NULL) {
                FreeBlock = XBlock;
                CurrentBlock = CurrentBlock->Next;
                delete FreeBlock;
            }
        }*/

       public:
        CollisionContainer() {
            /*
            this->Blockmap.Last = NULL;
            this->Blockmap.Next = NULL;
            this->Blockmap.YAxis = NULL;
            this->Blockmap.AxisIndex = 0;*/
        }
        ~CollisionContainer() {
            // FreeAxis();
        }

        inline void AddCollision(Game::Collision::BaseCollision* Collision) {
            Collision->_ParentContainer = this;
            Collision->RecalculateOccupiedBlocks();
        }
    };
}