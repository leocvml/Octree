#include <iostream>
#include <vector>
enum leave{
    ldf = 0,
    ldb = 1,
    luf = 2,
    lub = 3,
    rdf = 4,
    rdb = 5,
    ruf = 6,
    rub = 7
};
struct Point{
    int x,y,z;
    Point():x(-1),y(-1),z(-1){}
    Point(int a, int b, int c):x(a),y(b),z(c){}
};

/*     -       +
 x    left   right
 y    down   up
 z    front  back
*/
class octree{

    public:
        Point *p;
        octree *ldf, *rub;
        std::vector<octree*> children;

        octree(){
            p = new Point();
        }
        octree(int a, int b, int c){
            p = new Point(a,b,c);
        }
        octree(int x1, int y1, int z1, int x2, int y2, int z2){
            if(x1 > x2 || y1 > y2 || z1 > z2)
                return ;
            p = nullptr;
            ldf = new octree(x1,y1,z1);
            rub = new octree(x2,y2,z2);
            children.assign(8,nullptr);
            for(int i = leave::ldf; i <= leave::rub; i++){
                children[i] = new octree();
            }
        }

        int binarysearch(int x,int y,int z, int midx, int midy, int midz){
            int pos = -1;
            if(x < midx){
                if(y < midy){
                    if(z < midz){
                        pos = leave::ldf;
                    }
                    else{
                        pos = leave::ldb;
                    }
                }
                else{
                    if(z < midz){
                        pos = leave::luf;
                    }
                    else{
                        pos = leave::lub;
                    }
                }
            }
            else{
                if(y < midy){
                    if(z < midz){
                        pos = leave::rdf;
                    }
                    else{
                        pos = leave::rdb;
                    }
                }
                else{
                    if(z < midz){
                        pos = leave::ruf;
                    }
                    else{
                        pos = leave::rub;
                    }
                }
            }
            return pos;
        }

        void insert(int x, int y, int z){

            if (find(x,y,z)){
                std::cout <<"point already here" <<std::endl;
                return;
            }

            if(x < ldf->p->x || x > rub->p->x ||
            y < ldf->p->y || x > rub->p->y ||
            z < ldf->p->z || z > rub->p->z){
                std::cout <<"out of boundry" <<std::endl;
                return ;
            }

            int midx = (ldf->p->x + rub->p->x) / 2;
            int midy = (ldf->p->y + rub->p->y) / 2;
            int midz = (ldf->p->z + rub->p->z) / 2;
            int pos = binarysearch(x,y,z,midx,midy,midz);

            if(children[pos]->p == nullptr){
                children[pos]->insert(x,y,z);
            }
            else if(children[pos]->p->x == -1){
                delete children[pos];
                children[pos] = new octree(x,y,z);
                return;
            }
            else{
                int x_ = children[pos]->p->x;
                int y_ = children[pos]->p->y;
                int z_ = children[pos]->p->z;
                delete children[pos];
                children[pos] = nullptr; 
                if(pos == leave::ldf){
                    children[pos] = new octree(ldf->p->x,
                                            ldf->p->y,
                                            ldf->p->z,
                                            midx,
                                            midy,
                                            midz);
                }
                if(pos == leave::ldb){
                    children[pos] = new octree(ldf->p->x,
                                            ldf->p->y,
                                            midz+1,
                                            midx,
                                            midy,
                                            rub->p->z
                                            );
                }
                if(pos == leave::luf){
                    children[pos] = new octree(ldf->p->x,
                                            midy+1,
                                            ldf->p->z,
                                            midx,
                                            rub->p->y,
                                            midz
                                            );
                }
                if(pos == leave::lub){
                    children[pos] = new octree(ldf->p->x,
                                            midy+1,
                                            midz+1,
                                            midx,
                                            rub->p->y,
                                            rub->p->z);
                }
                if(pos == leave::rdf){
                    children[pos] = new octree(midx+1,
                                            ldf->p->y,
                                            ldf->p->z,
                                            rub->p->x,
                                            midy,
                                            midz);
                }
                if(pos == leave::rdb){
                    children[pos] = new octree(midx+1,
                                            ldf->p->y,
                                            midz+1,
                                            rub->p->x,
                                            midy,
                                            rub->p->z
                                            );
                }
                if(pos == leave::ruf){
                    children[pos] = new octree(midx+1,
                                            midy+1,
                                            ldf->p->x,
                                            rub->p->x,
                                            rub->p->y,
                                            midz);
                }
                if(pos == leave::rub){
                    children[pos] = new octree(midx+1,
                                            midy+1,
                                            midz+1,
                                            rub->p->x,
                                            rub->p->y,
                                            rub->p->z);
                }
                children[pos]->insert(x_,y_,z_);
                children[pos]->insert(x,y,z);
            }
        }
        
        bool find(int x, int y ,int z){
            if(x < ldf->p->x || x > rub->p->x ||
            y < ldf->p->y || y > rub->p->y ||
            z < ldf->p->z || z > rub->p->z){
                std::cout << "out of boundry" <<std::endl;
                return 0 ;
            }
            
            int midx = (ldf->p->x + rub->p->x) / 2;
            int midy = (ldf->p->y + rub->p->y) / 2;
            int midz = (ldf->p->z + rub->p->z) / 2;

            int pos = binarysearch(x,y,z,midx,midy,midz);

            if(children[pos]->p == nullptr){
                children[pos]->find(x,y,z);
            }
            else if(children[pos]->p->x == -1){
                return 0;
            }
            else{
                if(children[pos]->p->x == x && children[pos]->p->y == y && children[pos]->p->z ==z){
                    return 1;
                }
            }    
        }

        void traversal(){
            
            if(p == nullptr){
                for(int i = leave::ldf; i<= leave::rub;i++){
                    children[i]->traversal();
                }
            }
            else{ 
                if(p->x != -1){
                    std::cout << p->x <<" "<<p->y <<" "<<p->z <<std::endl;
                    return;
                }
            }
        }

};


int main(){
    octree t(0,0,0,10,10,10);

    t.insert(1,1,1);
    t.insert(1,1,2);
    t.insert(3,3,3);
    t.insert(8,8,8);
    std::cout << (t.find(3,3,3) ? "yes":"no")<<std::endl;  
    std::cout << (t.find(3,2,3) ? "yes":"no")<<std::endl;  
    t.traversal();
 
  
    return 0;


}