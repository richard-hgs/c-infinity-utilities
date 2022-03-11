#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#endif

extern "C++" class INFINITY_UTILS_API ArrayObject {
    private:
        size_t arraySizeBuffer;
        size_t arraySize;
        void** arrayObjects;
    public:
    ArrayObject(size_t initialBufferSize = 10) {
        this->arraySizeBuffer = initialBufferSize;
        this->arraySize = 0;
        this->arrayObjects = (void**) malloc(sizeof(void*) * arraySizeBuffer);
    }

    ~ArrayObject() {
        free(arrayObjects);
    }

    void add(Object object) {
        this->addPtr(new Object(object));
    }

    void addPtr(Object* object) {
        // std::cout << "arraySize: " << arraySize << " - maxArraySize: " << arraySizeBuffer << std::endl;
        if (this->arraySize >= arraySizeBuffer) {
            if (this->arraySizeBuffer < 1000) {
                this->arraySizeBuffer = this->arraySizeBuffer * 2;
            } else {
                this->arraySizeBuffer = this->arraySizeBuffer + 10000;
            }

            std::cout << "ArrayObject - maxBufferSizeReached increasingBuffer to " << this->arraySizeBuffer << std::endl;

            void** tempArrayObjects = (void**) realloc(this->arrayObjects, sizeof(void*) * this->arraySizeBuffer);
            // Prevent memory leak of original allocation if nullptr returns
            if (tempArrayObjects != NULL) {
                this->arrayObjects = tempArrayObjects;
            }
        }

        this->arrayObjects[arraySize] = object;
        arraySize++;
    }

    Object get(size_t index) {
        return *(Object*) this->arrayObjects[index];
    }

    Object* getPtr(size_t index) {
        return (Object*)this->arrayObjects[index];
    }

};