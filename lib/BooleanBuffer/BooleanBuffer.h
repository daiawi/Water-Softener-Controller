#pragma once

// Simple Boolean Buffer Class - Averages Boolean Variables over a Window
class BooleanBuffer {
  public:
    int size, count;
    bool isStableTrue = 0;    

    // Init Count with 1, size/2, or size-1
    BooleanBuffer(int size) : size(size), count(size-1) {}

    void add(bool condition) { 
      if (condition){
        count = (count+1) - ((count+1)%size == 0);
      }
      else{
        count = (count-1) + ((count-1)%size == 0);
      }
      isStableTrue = count > size/2;
    }
};