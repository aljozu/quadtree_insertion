//
// Created by lojaz on 16/09/2022.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <cmath>
#define width 900
#define height 900
#include <iostream>
using namespace std;
using namespace sf;

RectangleShape draw_line(Vector2f p1, Vector2f p2){
    float modulo = sqrt(((p2.x - p1.x)*(p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)));
    auto angle = float((atan2(p1.y - p2.y, p1.x - p2.x)) * (180 / M_PI));
    RectangleShape line(Vector2f(modulo, 1));
    line.setPosition(p1.x, p1.y);
    line.setRotation(180-angle);
    line.setFillColor(Color::Magenta);
    return line;
}

struct Point{
    sf::CircleShape particle;
    sf::RectangleShape horizontal_line, vertical_line;
    float x, y;
    Point() = default;
    Point(float x, float y){
        this->x= x;
        this->y = y;
        particle = CircleShape(3);
        particle.setPosition(x, y);
        particle.setFillColor(Color::Cyan);
    }
};

struct BoundingBox{
    Point p0, p1;
};

struct node{
    vector<node*> nodes ={nullptr, nullptr, nullptr, nullptr};
    node* parent = nullptr;
    BoundingBox bb;
    Point data;

    void set_bb(int index){
        if(index == 5){
            bb.p0 = Point(0,0);
            bb.p1 = Point(width,height);
            return;
        }else if (index == 2){
            bb.p0 = parent->bb.p0;
            bb.p1 = Point(parent->data.x,parent->data.y);
            return;
        } else if(index == 3){
            bb.p0 = Point(parent->data.x,parent->bb.p0.y);
            bb.p1 = Point(parent->bb.p1.x, parent->data.y);
            return;
        } else if(index == 0){
            bb.p0 = Point(parent->bb.p0.x, parent->data.y);
            bb.p1 = Point(parent->data.x,parent->bb.p1.y);
            return;
        } else if(index == 1) {
            bb.p0 = Point(parent->data.x,parent->data.y);
            bb.p1 = parent->bb.p1;
            return;
        } else {
            return;
        }
    }
};

class quadtree{
    node* root = nullptr;

    node* alloc_node(float x, float y){
        node * ptr = new node;
        ptr->data = Point(x, y);
        return ptr;
    }

    int where_i(Point data, float x, float y){
        return 2*(y<data.y)+(x>=data.x);
    }

    node* insert_p(node* a_node, float x, float y){

        if(a_node == nullptr)
            return alloc_node(x, y);
        int index = where_i(a_node->data, x, y);

        if(a_node->nodes[index] == nullptr){
            a_node->nodes[index] = insert_p(a_node->nodes[index], x, y);
            a_node->nodes[index]->parent = a_node;
            a_node->nodes[index]->set_bb(index);
        }else{
            insert_p(a_node->nodes[index], x, y);
        }
        return a_node;
    }
public:
    quadtree() {
        root = nullptr;
    }
    ~quadtree() = default;

    void insert(float x, float y){
        if(root == nullptr){
            root = alloc_node(x, y);
            root->set_bb(5);
            return;
        }
        insert_p(root, x, y);
    }

    void print_tree(RenderWindow& window){
        queue<node*> q;
        q.push(root);
        while(!q.empty()){
            node* no = q.front();
            window.draw(no->data.particle);
            sf::Vertex vertical[2] = {Vector2f(no->data.x, no->bb.p0.y), Vector2f(no->data.x, no->bb.p1.y)};
            sf::Vertex horizontal[2] = {Vector2f(no->bb.p0.x, no->data.y), Vector2f(no->bb.p1.x, no->data.y)};
            window.draw(vertical, 2, sf::Lines);
            window.draw(horizontal, 2, sf::Lines);
            q.pop();
            for(auto &x: no->nodes){
                if(x != nullptr)
                    q.push(x);
            }
        }
    }

    bool is_empty(){
        if(root == nullptr) return true;
        return false;
    }
};


#endif //QUADTREE_QUADTREE_H
