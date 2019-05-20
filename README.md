# php-roaring_bitmap-extension

* 对CRoaring进行封装的PHP扩展库，目前仅支持 PHP 7.0 以上
* githup上的[CRoaring](https://github.com/RoaringBitmap/CRoaring)
* 文件夹里包含的roaring.h roaring.hh roaring.c是CRoaring的0.2.60版本，用amalgamation.sh生成的。因此编译本库不依赖其他库。

# Install
* 下载后将本目录改名roaring_bitmap并放进php的源代码目录，譬如php-7.xxx/ext/roaring_bitmap
* 进入这个目录，并且运行phpize && ./configure && make install
* 修改php.ini，将extension=roaring_bitmap.so加进去

# Example(32 bit):
```
$roaring = new roaring_bitmap;

$roaring->addMany(array(1,2,3));

$n = 3;
printf("contains %d: %d\n", $n, $roaring->contains($n));

$roaring2 = new roaring_bitmap;
$roaring2->addMany(array(10,11,12));

$roaring->or($roaring2);
printf("cardinality:%u\n", $roaring->cardinality());
$n=10;
printf("contains %d: %d\n", $n, $roaring->contains($n));

printf("roaring2 isSubset:%d\n", $roaring2->isSubSet($roaring));

printf("roaring values:");
for($roaring->iterator_begin(); !$roaring->iterator_end(); $roaring->iterator_advance()){
        printf("%u,", $roaring->iterator_value());
}
printf("\n");

$vals = $roaring->toArray();
print_r($vals);
```


# Example(64 bit)
```
$roaring = new roaring_bitmap64;

$roaring->addMany(array(9000000000000000100,9000000000000000101,9000000000000000102));

$n = 9000000000000000102;
printf("contains %d: %d\n", $n, $roaring->contains($n));

$roaring2 = new roaring_bitmap64;
$roaring2->addMany(array(9000000000000000110,9000000000000000111,9000000000000000112));

$roaring->or($roaring2);
printf("cardinality:%u\n", $roaring->cardinality());
$n=10;
printf("contains %d: %d\n", $n, $roaring->contains($n));

printf("roaring2 isSubset:%d\n", $roaring2->isSubSet($roaring));

printf("roaring values:");
for($roaring->iterator_begin(); !$roaring->iterator_end(); $roaring->iterator_advance()){
        printf("%u,", $roaring->iterator_value());
}
printf("\n");

$vals = $roaring->toArray();
print_r($vals);
```


# roaring_bitmap API
*  void roaring_bitmap::__construct()
>    Create the roaring_bitmap object 

*  long roaring_bitmap::add(long $n)
>    Add value n 

*  bool roaring_bitmap::addChecked(long $n)
>    Add value n. Returns true if a new value was added, false if the value was already existing 

*  void roaring_bitmap::addRange(long $x, long $y)
>    add if all values from x (included) to y (excluded) 

*  long roaring_bitmap::addMany(array $arr)
>    Add value from array $arr 

*  long roaring_bitmap::remove(long $n)
>    Remove value n 

*  bool roaring_bitmap::removeChecked(long $n)
>    Remove value n. Returns true if a new value was removed, false if the value was not existing. 

*  long roaring_bitmap::maximum()
>    Return the largest value (if not empty) 

*  long roaring_bitmap::minimum()
>    Return the smallest value (if not empty) 

*  bool roaring_bitmap::contains(long $n)
>    Check if value n is present 

*  bool roaring_bitmap::contains(long $x, long $y)
>    Check if all values from x (included) to y (excluded) are present 

*  long roaring_bitmap::rank(long $n)
>    Returns the number of integers that are smaller or equal to n. 

*  long roaring_bitmap::select(long $rank)
>    If the size of the roaring bitmap is strictly greater than rank, then 
>    this function returns the element of given rank. 
>    Otherwise, it returns false. 

*  bool roaring_bitmap::flip(long $begin, long $end)
>    compute the negation of the roaring bitmap within a specified interval. areas outside the range are passed through unchanged. 

*  bool roaring_bitmap::isEmpty()
>    Returns true if the bitmap is empty (cardinality is zero). 

*  long roaring_bitmap::cardinality()
>    Get the cardinality of the bitmap (number of elements). 

*  long roaring_bitmap::getSizeInBytes([bool $portable = false])
>    How many bytes are required to serialize this bitmap.
>    Setting the portable flag to false enable a custom format that can save 
>    space compared to the portable format (e.g., for very sparse bitmaps).

*  long roaring_bitmap::replace(roaring_bitmap $another)
>    Copies the content of the provided bitmap, and discard the current content. 

*  long roaring_bitmap::sub(roaring_bitmap $another)
>    Compute the difference between the current bitmap and the provided bitmap, 
>    writing the result in the current bitmap. The provided bitmap is not modified.

*  long roaring_bitmap::and(roaring_bitmap $another)
>    Copies the content of the provided bitmap, and discard the current content. 

*  long roaring_bitmap::or(roaring_bitmap $another)
>    Compute the union between the current bitmap and the provided bitmap, 
>    writing the result in the current bitmap. The provided bitmap is not modified.

*  long roaring_bitmap::xor(roaring_bitmap $another)
>    Compute the symmetric union between the current bitmap and the provided bitmap,
>    writing the result in the current bitmap. The provided bitmap is not modified. 

*  long roaring_bitmap::and_cardinality(roaring_bitmap $another)
>    Computes the size of the intersection between two bitmaps. 

*  long roaring_bitmap::or_cardinality(roaring_bitmap $another)
>    Computes the size of the union between two bitmaps. 

*  long roaring_bitmap::andnot_cardinality(roaring_bitmap $another)
>    Computes the size of the difference (andnot) between two bitmaps. 

*  long roaring_bitmap::xor_cardinality(roaring_bitmap $another)
>    Computes the size of the symmetric difference (andnot) between two bitmaps. 

*  double roaring_bitmap::jaccard_index(roaring_bitmap $another)
>    Computes the Jaccard index between two bitmaps. (Also known as the Tanimoto distance, 
>    or the Jaccard similarity coefficient)
>    The Jaccard index is undefined if both bitmaps are empty. 

*  bool roaring_bitmap::isSubset(roaring_bitmap $another)
>    Returns true if the bitmap is subset of the other. 

*  bool roaring_bitmap::isStrictSubset(roaring_bitmap $another)
>    Returns true if the bitmap is strict subset of the other. 

*  bool roaring_bitmap::iterator_begin()
>    Iterate over the bitmap elements. Move the iterator to the first value. 

*  bool roaring_bitmap::iterator_end()
>    Return false means that the iteration should continue while true means that it should stop. 

*  bool roaring_bitmap::iterator_advance()
>    Move the iterator to the next. 

*  long roaring_bitmap::iterator_value()
>    Return current iterator value. 

*  array roaring_bitmap::toArray()
>    Convert the bitmap to an array and return it. array size is: this->cardinality() 

*  string roaring_bitmap::toString()
>    Print the content of the bitmap into a string and return it. 

*  bool roaring_bitmap::write([bool $portable = false])
>    write a bitmap to a char buffer. Returns how many bytes were written which should be getSizeInBytes(). 
>    Setting the portable flag to false enable a custom format that can save space compared to the portable format 
>    (e.g., for very sparse bitmaps). 

*  bool roaring_bitmap::read([bool $portable = false])
>    read a bitmap from a serialized version. Setting the portable flag to false enable a custom format that 
>    can save space compared to the portable format (e.g., for very sparse bitmaps). 

*  bool roaring_bitmap::getCopyOnWrite()
>    Whether or not copy and write is active. 

*  bool roaring_bitmap::setCopyOnWrite()
>    Whether or not we apply copy and write. 

*  bool roaring_bitmap::removeRunCompression()
>    Remove run-length encoding even when it is more space efficient return whether a change was applied. 

*  bool roaring_bitmap::runOptimize()
>    convert array and bitmap containers to run containers when it is more efficient; also convert from run 
>    containers when more space efficient.  Returns true if the result has at least one run container. 
>    Additional savings might be possible by calling shrinkToFit(). 

*  long roaring_bitmap::shrinkToFit()
>    If needed, reallocate memory to shrink the memory usage. Returns the number of bytes saved. 

# roaring_bitmap64 API
*  void roaring_bitmap64::__construct()
>    Create the roaring_bitmap64 object 

*  long roaring_bitmap64::add(string $n)
>    Add value n(numeric string) 

*  bool roaring_bitmap64::addChecked(string $n)
>    Add value n. Returns true if a new value was added, false if the value was already existing 

*  long roaring_bitmap64::addMany(array $arr)
>    Add value from array $arr 

*  long roaring_bitmap64::remove(string $n)
>    Remove value n 

*  bool roaring_bitmap64::removeChecked(string $n)
>    Remove value n. Returns true if a new value was removed, false if the value was not existing. 

*  string roaring_bitmap64::maximum()
>    Return the largest value (if not empty) 

*  string roaring_bitmap64::minimum()
>    Return the smallest value (if not empty) 

*  bool roaring_bitmap64::contains(string $n)
>    Check if value n is present 

*  string roaring_bitmap64::rank(string $n)
>    Returns the number of integers that are smaller or equal to n. 

*  string roaring_bitmap64::select(string $rank)
>    If the size of the roaring bitmap is strictly greater than rank, then 
>    this function returns the element of given rank. 
>    Otherwise, it returns false. 

*  bool roaring_bitmap64::flip(string $begin, string $end)
>    compute the negation of the roaring bitmap within a specified interval. 
>    areas outside the range are passed through unchanged. 

*  bool roaring_bitmap64::isEmpty()
>    Returns true if the bitmap is empty (cardinality is zero). 

*  string roaring_bitmap64::cardinality()
>    Get the cardinality of the bitmap (number of elements). 

*  string roaring_bitmap64::getSizeInBytes([bool $portable = false])
>    How many bytes are required to serialize this bitmap.
>    Setting the portable flag to false enable a custom format that can save 
>    space compared to the portable format (e.g., for very sparse bitmaps).

*  long roaring_bitmap64::replace(roaring_bitmap64 $another)
>    Copies the content of the provided bitmap, and discard the current content. 

*  long roaring_bitmap64::sub(roaring_bitmap64 $another)
>    Compute the difference between the current bitmap and the provided bitmap, 
>    writing the result in the current bitmap. The provided bitmap is not modified.

*  long roaring_bitmap64::and(roaring_bitmap64 $another)
>    Copies the content of the provided bitmap, and discard the current content. 

*  long roaring_bitmap64::or(roaring_bitmap64 $another)
>    Compute the union between the current bitmap and the provided bitmap, 
>    writing the result in the current bitmap. The provided bitmap is not modified.

*  long roaring_bitmap64::xor(roaring_bitmap64 $another)
>    Compute the symmetric union between the current bitmap and the provided bitmap,
>    writing the result in the current bitmap. The provided bitmap is not modified. 

*  bool roaring_bitmap64::isSubset(roaring_bitmap64 $another)
>    Returns true if the bitmap is subset of the other. 

*  bool roaring_bitmap64::isStrictSubset(roaring_bitmap64 $another)
>    Returns true if the bitmap is strict subset of the other. 

*  bool roaring_bitmap64::iterator_begin()
>    Iterate over the bitmap elements. Move the iterator to the first value. 

*  bool roaring_bitmap64::iterator_end()
>    Return false means that the iteration should continue while true means that it should stop. 

*  bool roaring_bitmap64::iterator_advance()
>    Move the iterator to the next. 

*  string roaring_bitmap64::iterator_value()
>    Return current iterator value. 

*  array roaring_bitmap64::toArray()
>    Convert the bitmap to an array and return it. array size is: this->cardinality() 

*  string roaring_bitmap64::toString()
>    Print the content of the bitmap into a string and return it. 

*  bool roaring_bitmap64::write([bool $portable = false])
>    write a bitmap to a char buffer. Returns how many bytes were written which should be getSizeInBytes(). 
>    Setting the portable flag to false enable a custom format that can save space compared to the portable format 
>    (e.g., for very sparse bitmaps). 

*  bool roaring_bitmap64::read([bool $portable = false])
>    read a bitmap from a serialized version. Setting the portable flag to false enable a custom format that 
>    can save space compared to the portable format (e.g., for very sparse bitmaps). 

*  bool roaring_bitmap64::getCopyOnWrite()
>    Whether or not copy and write is active. 

*  bool roaring_bitmap64::setCopyOnWrite()
>    Whether or not we apply copy and write. 

*  bool roaring_bitmap64::removeRunCompression()
>    Remove run-length encoding even when it is more space efficient return whether a change was applied. 

*  bool roaring_bitmap64::runOptimize()
>    convert array and bitmap containers to run containers when it is more efficient; also convert from run 
>    containers when more space efficient.  Returns true if the result has at least one run container. 
>    Additional savings might be possible by calling shrinkToFit(). 

*  long roaring_bitmap64::shrinkToFit()
>    If needed, reallocate memory to shrink the memory usage. Returns the number of bytes saved. 


