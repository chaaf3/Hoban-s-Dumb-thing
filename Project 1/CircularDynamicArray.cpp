
#include <iostream>
#include <string>
#include <math.h>
#include <random>

template <class T>
class CircularDynamicArray {
	private:

		int size;
		int cap;
		int head;
		T *list;
		T trash;

		
		void upsize() {

			int newCap = this->cap*2;
			T *newList = new T[newCap];

			for (int i=0; i<size; i++) {

				newList[i] = this->list[this->relInd(i)];
			
			}

			delete [] this->list;

			this->list=newList;
			this->cap=newCap;
			this->head=0;
		}

		
		void downsize() {

			if (size==0) return;

			int newCap = this->cap/2;
			T *newList = new T[newCap];

			for (int i=0;i<size;i++) {

				newList[i] = this->list[this->relInd(i)];

			}

			delete [] this->list;

			this->list=newList;
			this->cap=newCap;
			this->head=0;
		}

		int relInd(int index) {
            
				return (this->head+index)%this->cap;
			
		}


		/**
		 * Used by quick sort and quick select
		 * This function selects a pivot, puts every element bigger than it
		 * on the right side (larger index) and everything smaller than it on the
		 * left side (smaller index), and return's the pivot's updated position.
		 *
		 * @param min: the starting index of the portion of the array to be partitioned
		 * @param max: the ending index of the portion of the array to be partitioned
		 * @return: returns the pivot's updated position
		 */
		int partition(int min, int max, int pivotIndex) {

            if (max <= min) return min;

			T swap=this->list[relInd(min)];
			this->list[relInd(min)]=this->list[relInd(pivotIndex)];
			this->list[relInd(pivotIndex)]=swap;


			T pivot = this->list[relInd(min)];
			min++;
			int i = min;

			for (;;) {

				if (this->list[relInd(i)] <= pivot) {

					this->list[relInd(i - 1)]=this->list[relInd(i)];
					i++;

					if (i > max) {

						this->list[relInd(i-1)]=pivot;
						return i - 1;

					}
				}
				else if (this->list[relInd(i)] > pivot) {

					swap = this->list[relInd(max)];
					this->list[relInd(max)] = this->list[relInd(i)];
					this->list[relInd(i)] = swap;
					max--;

					if (i>max) {

						list[relInd(i-1)]=pivot;
						return i-1;

					}
				}
			}
        }

        /**
         * This function is used in WCSelect
         * This finds the median of small portions of the array, and then
         * finds the median of those medians, and returns that median's
         * index. This is used to ensure a good partition.
         *
         * @param min: minimum index when looking for a median
         * @param max: maximum index to look for a median
         */
        int meds(int min, int max) {

            int medianSize = 5;
            int numMedians = (max - min)/medianSize+1;
            int* medians_i = new int[numMedians];
            int i,j,d;
            int median;

            for (j = 0, i = min + medianSize; i - 1 <= max; j++, i += medianSize) {
                
                selectionSort(i-medianSize,i-1);
                medians_i[j] = ((i-1)+(i-medianSize))/2;
                
            }

            d=(max-min+1)%medianSize;

            if (d) { 

                mergeSort(i-medianSize,i-1-(medianSize-d));
                medians_i[j] = (i-medianSize+i-1-(medianSize-d))/2;

            }

            selectionSortArr(medians_i,0,(max-min)/medianSize);
            median = medians_i[((max-min)/medianSize)/2];

            delete [] medians_i;
            return median;
        }

		void mergeSort(int min, int max) {
			if (min >= max) return;

			int mid = (max + min + 1)/2;
			mergeSort(min,mid-1);
			mergeSort(mid,max);

			zip(min,mid,max);
		}

		
		void zip(int min, int mid, int max) {

			T* temp = new T[max-min+1];
			int smin=min; //starting min

			int rmid=mid; //right mid
			int lmid=mid-1;//left mid
			int i=0;

			for (;min<=lmid && max>=rmid;i++) {

				if (this->list[relInd(rmid)] < this->list[relInd(min)]) {
					temp[i] = this->list[relInd(rmid)];
					rmid++;
				}
				else {
					temp[i] = this->list[relInd(min)];
					min++;
				}
			}

			for (;min <= lmid;i++, min++) {
				
				temp[i] = this->list[relInd(min)];

			}

			for (;max >= rmid;i++, rmid++) {

				temp[i] = this->list[relInd(rmid)];

			}

			for (int j = 0; j < i; j++) {

				this->list[relInd(j+smin)]=temp[j];

			}

            delete [] temp;
		}


	public:

		CircularDynamicArray() {
			this->size=0;
			this->cap=2;
			this->head=0;
			this->list=new T[cap];

			this->trash=0;
		}

		CircularDynamicArray(int s) {
			this->size=s;
			this->cap=s;
			this->head=0;
			this->list=new T[cap];

			this->trash=0;
		}

		~CircularDynamicArray() {
			delete [] this->list;
		}

		CircularDynamicArray(CircularDynamicArray &original) {
			this->size = original.size;
			this->cap = original.cap;
			this->head = 0;
			this->list = new T[this->cap];

			for (int i=0;i<this->size;i++) {

				this->list[i] = original[i];

			}
		}

		CircularDynamicArray& operator=(CircularDynamicArray& original) {
			this->size = original.size;
			this->cap = original.cap;
			this->head = 0;
			this->list = new T[this->cap];

			for (int i=0;i<this->size;i++) {
				this->list[i] = original[i];
			}
			return *this;
		}

		T& operator[](int index) {
			if (index < 0 || index > this->size-1) {

				std::cout << "OOB: " << index << std::endl;

				return this->trash;
			}
			else return this->list[this->relInd(index)];
		}

		void addEnd(T data) {

			if (this->size==this->cap) {

				this->upsize();
				this->addEnd(data);

			}
			else {

				this->list[(this->head+this->size)%this->cap]=data;
				this->size++;

			}
		}

		
		void addFront(T data) {

			if (this->size==this->cap) {

				this->upsize();
				this->addFront(data);

			}

			else {

				this->head = (this->head+this->cap-1)%this->cap;
				this->list[this->head]=data;
				this->size++;

			}
		}

		void delEnd() {

			T data = this->list[this->relInd(this->size-1)];
			this->size--;

			if (((double) this->size)/((double) this->cap) <= .25) {

				this->downsize();

			}
		}

		void delFront() {

			T data = this->list[this->head];
			this->head=(this->head+1)%this->cap;
			this->size--;

			if (((double) this->size)/((double) this->cap) <= .25) {

				this->downsize();

			}
		}

		int length() {
			return this->size;
		}

		int capacity() {
			return this->cap;
		}

		void clear() {
			delete [] this->list;
			this->size=0;
			this->cap=2;
			this->head=0;
			this->list=new T[cap];
		}


		T QuickSelect(int k) {
			k--;
			if (k<0 || k>this->size-1) return this->trash;

			int pivotIndex = 0;
			
			int max = size - 1;
			int min = 0;

			pivotIndex = partition(min, max, min);

			
			while (pivotIndex != k) {
				if (pivotIndex < k) {
					
					min = pivotIndex + 1;

				}
				else {
					
					max = pivotIndex - 1;

				}

				pivotIndex = partition(min, max, min);
			}
			
			return this->list[relInd(k)];
		}

		
		T WCSelect(int k) {
			k--;
			if (k<0 || k>this->size-1) return this->trash; 
			int pivotIndex=0;
			
			int max=this->size-1;
			int min=0;

            pivotIndex = meds(min, max);
			pivotIndex = partition(min, max, pivotIndex);

			//count while loop
			while (pivotIndex!=k) {
				if (pivotIndex<k) min = pivotIndex+1;
				else max = pivotIndex-1;

                
                pivotIndex = meds(min,max);
                pivotIndex = partition(min, max, pivotIndex);
			}
			
			return this->list[relInd(pivotIndex)];
		}

        
		void selectionSort(int min, int max) {
            int min_i;
            T minVal;

            for (;min<max;min++) {
                min_i = min;
                T minVal = this->list[relInd(min_i)];
                
                for (int i=min+1;i<max;i++) {
                    if (this->list[relInd(i)] < minVal) {
                        minVal = this->list[relInd(i)];
                        min_i=i;
                    }
                }
                
                this->list[relInd(min_i)] = this->list[relInd(min)];
                this->list[relInd(min)] = minVal;
            }
		}

		
        
		
        void selectionSortArr(int* arr, int min, int max) { 
            int min_i;
            T minVal;
            int swap;

            for (int i=min;i<max;min++,i++) {
                
                min_i = min;
                minVal = this->list[relInd(arr[min_i])];

                for (int j=min + 1;j<=max;j++) {
                    if (this->list[relInd(arr[j])] < minVal) {
                        min_i = j;
                        minVal = this->list[relInd(arr[min_i])];
                    }
                }
                swap = arr[min];
                arr[min] = arr[min_i];
                arr[min_i] = swap;
            }
        }
		
		void stableSort() {
			mergeSort(0,size-1);
		}

		int linearSearch(T value) {
			for (int i=0;i<this->size;i++) {
				if (value == this->list[relInd(i)]) return i;
			}
			return -1;
		}

		int binSearch(T value) {
			int min=0;
			int max=this->size-1;
			int mid=(max - min)/2;
			for (;value != this->list[relInd(mid)] && max > min;) {
				if (value < this->list[relInd(mid)]) {
					max = mid-1;
					mid = (max + min)/2;
				}
				else {
					min = mid + 1;
					mid = (max + min)/2;
				}
			}
			if (value == this->list[relInd(mid)]) return mid;
			else return -1;
		}
};

// Citation

/***************************************************************************************
*    Author: Asa Dillahunty
*    Date: 9/18/2022
*    Availability: https://github.com/asa-dillahunty/CS201-Data-Structures-Library/blob/master/Phase1/CircularDynamicArray.cpp
*
***************************************************************************************/
/*
Used as a general reference for this project and learning new programing techniques.

https://github.com/asa-dillahunty/CS201-Data-Structures-Library/blob/master/Phase1/CircularDynamicArray.cpp

The Make File  was not working properly on my PC and I am unsure why, it keeps saying that makefile is not found.
*/