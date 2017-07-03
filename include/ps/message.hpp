#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <cstring>
#include <iostream>
#include <vector>

using std::vector;

namespace ps {

#define DEFAULT_THRESHOLD 0.00001

/** 
  *@brief Message passed by in the system. k-v keys form.
  *       It can be serialize to or deserialize from the 1-D array
  *       with the format 
  *         range style: [total_size, k-v_pairs, -1, k0, v0, k1, v1, ... , kn, vn]
  *         non-range style: [total_size, k-v_pairs, rangeFrom, v0, v1, v2, ... , vn]
  */
template <typename T>
class Message {
  public:
    Message() : size_(0), range_(false) {}
    Message(T* arr, int kv_num, int rangeFrom=-1, int size=-1, double threshold=DEFAULT_THRESHOLD);
    int getSerialSize() const;
    T* serialize() const;
    void deserialize(const T* fromBuff);
    void set(int index, T value);
    void add(T value, double threshold=DEFAULT_THRESHOLD);
    T get(int index) const;
    T* getRawArray() const;
    // the number of k-v pairs
    int count() const { return values_.size(); }
    void show(int num=-1) const;
  private:
    int size_;    // the actual size of the whole array
    bool range_;  // determine if this is a range message
    // if range_ is true, keys_[0] == [rangeFrom]; else keys_ is the array of keys.
    vector<int> keys_;
    vector<T> values_;
};

template <typename T>
Message<T>::Message(T* arr, int kv_num, int rangeFrom, int size, double threshold) {
  size_ = size == -1 ? kv_num : size;
  if (rangeFrom != -1) {
    range_ = true;
    keys_.push_back(rangeFrom);
    values_.reserve(kv_num);
    for (int i = 0; i < kv_num; ++i) {
      values_.push_back(arr[i]);
    }
  } else {
    range_ = false;
    keys_.reserve(kv_num);
    values_.reserve(kv_num);
    for (int i = 0; i < kv_num; ++i) {
      if (arr[i] > threshold || arr[i] < -1 * threshold) {
        keys_.push_back(i);
        values_.push_back(arr[i]);
      }
    }
    keys_.resize(count());
    values_.resize(count());
  }
}

template <typename T>
int Message<T>::getSerialSize() const {
  if (range_) {
    // [size, kv-num, from, v0, v1, ... , vn]
    return 3 + count();
  } else {
    // [size, kv-num, -1, k0, v0, k1, v1, ..., kn, vn]
    return 3 + count() * 2;
  }
}

template <typename T>
T* Message<T>::serialize() const {
  T* toBuff = new T[getSerialSize()];
  toBuff[0] = size_;
  toBuff[1] = (T) count();
  if (range_) {
    toBuff[2] = keys_[0];
    for (int i = 0; i < count(); ++i) {
      toBuff[i + 2] = values_[i];
    }
  } else {
    toBuff[2] = -1;
    for (int i = 0; i <= count(); ++i) {
      toBuff[2 * i + 2] = (T) keys_[i];
      toBuff[2 * i + 3] = values_[i];
    }
  }
  return toBuff;
}

template <typename T>
void Message<T>::deserialize(const T* fromBuff) {
  if (fromBuff == NULL) {
    return;
  }
  size_ = fromBuff[0];
  int kvs = fromBuff[1];
  if (kvs <= 0) {
    range_ = false;
    keys_ .resize(0);
    values_.resize(0);
    return;
  }
  if (fromBuff[2] == -1) {
    range_ = false;
    keys_.reserve(kvs);
    values_.reserve(kvs);
    for (int i = 0; i < kvs; ++i) {
      keys_.push_back((int) fromBuff[2 * i + 2]);
      values_.push_back(fromBuff[2 * i + 3]);
    }
  } else {
    range_ = true;
    values_.reserve(kvs);
    keys_[0] = fromBuff[1];
    for (int i = 0; i < kvs; ++i) {
      values_.push_back(fromBuff[i + 2]);
    }
  }
}

template <typename T>
void Message<T>::set(int index, T value) {
  if (index >= size_) return;
  keys_.push_back(index);
  values_.push_back(value);
}

template <typename T>
void Message<T>::add(T value, double threshold) {
  if (threshold > (double) value && (double) value > -1 * threshold) {
    // do nothing
    return;
  }
  ++size_; 
  keys_.push_back(size_);
  values_.push_back(value);
}

template <typename T>
T Message<T>::get(int index) const {
  if (index >= size_) {
    //  out of range
    return 0;
  }
  for (int i = 0; i < count(); ++i) {
    if (keys_[i] == i) {
      return values_[i];
    }
  }
  return 0;
}

template <typename T>
T* Message<T>::getRawArray() const {
  T* tmpBuf = new T[size_];
  memset(tmpBuf, 0, size_);
  for (int i = 0; i < count(); ++i) {
    if (range_) {
      tmpBuf[keys_[0] + i] = values_[i];
    } else {
      tmpBuf[keys_[i]] = values_[i];
    }
  } 
  return tmpBuf;
}

template <typename T>
void Message<T>::show(int num) const {
  if (num == -1) {  // only show 10 items
    num = count() > 10 ? 10 : count();
  } else {
    num = num > count() ? count() : num;
  }
  if (range_) {
    for (int i = 0; i < num; ++i) {
      std::cout << (keys_[0] + i) << ": " << values_[i] << std::endl;
    }
  } else {
    for (int i = 0; i < num; ++i) {
      std::cout << keys_[i] << ": " << values_[i] << std::endl;
    }
  }
}

}
#endif
