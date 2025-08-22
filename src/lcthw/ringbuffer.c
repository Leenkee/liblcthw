#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>

RingBuffer *RingBuffer_create(int length)
{
    RingBuffer *buffer = calloc(1, sizeof(RingBuffer));		//声明一个ringbuffer
    buffer->length = length + 1;
    buffer->start = 0;
    buffer->end = 0;
    buffer->buffer = calloc(buffer->length, 1);				//在堆上为ringbuffer分配length个大小为1字节的单位

    return buffer;
}

void RingBuffer_destroy(RingBuffer * buffer)
{
    if (buffer) {
        free(buffer->buffer);
        free(buffer);
    }
}

int RingBuffer_write(RingBuffer * buffer, char *data, int length)
{
    if (RingBuffer_available_data(buffer) == 0) {			//如果队列满了
        buffer->start = buffer->end = 0;					//重头写入
    }

    check(length <= RingBuffer_available_space(buffer),		
            "Not enough space: %d available, %d request",	//队列单元格数不足
            RingBuffer_available_data(buffer), length);

    void *result = memcpy(RingBuffer_ends_at(buffer), data, length);
    check(result != NULL, "Failed to write data into buffer.");

    RingBuffer_commit_write(buffer, length);				//尾部写入后，更新尾部位置

    return length;
error:
    return -1;
}

int RingBuffer_read(RingBuffer * buffer, char *target, int amount)
{
    check_debug(amount <= RingBuffer_available_data(buffer),
            "Not enough in the buffer: has %d, needs %d",
            RingBuffer_available_data(buffer), amount);

    void *result = memcpy(target, RingBuffer_starts_at(buffer), amount);//将最久的即开头的读出
    check(result != NULL, "Failed to write buffer into data.");

    RingBuffer_commit_read(buffer, amount);

    if (buffer->end == buffer->start) {					//队列空
        buffer->start = buffer->end = 0;
    }

    return amount;
error:
    return -1;
}

bstring RingBuffer_gets(RingBuffer * buffer, int amount)
{
    check(amount > 0, "Need more than 0 for gets, you gave: %d ",
            amount);
    check_debug(amount <= RingBuffer_available_data(buffer),
            "Not enough in the buffer.");

    bstring result = blk2bstr(RingBuffer_starts_at(buffer), amount);
    check(result != NULL, "Failed to create gets result.");
    check(blength(result) == amount, "Wrong result length.");

    RingBuffer_commit_read(buffer, amount);
    assert(RingBuffer_available_data(buffer) >= 0
            && "Error in read commit.");

    return result;
error:
    return NULL;
}
