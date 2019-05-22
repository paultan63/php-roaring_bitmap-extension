--TEST--
Check for roaring_bitmap
--SKIPIF--
<?php if (!extension_loaded("roaring_bitmap")) print "skip"; ?>
--FILE--
<?php
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
?>
--EXPECT--
contains 3: 1
cardinality:6
contains 10: 1
roaring2 isSubset:1
roaring values:1,2,3,10,11,12,
Array
(
    [0] => 1
    [1] => 2
    [2] => 3
    [3] => 10
    [4] => 11
    [5] => 12
)
