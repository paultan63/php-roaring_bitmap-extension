--TEST--
Check for roaring_bitmap64
--SKIPIF--
<?php if (!extension_loaded("roaring_bitmap")) print "skip"; ?>
--FILE--
<?php
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
?>
--EXPECT--
contains 9000000000000000102: 1
cardinality:6
contains 10: 0
roaring2 isSubset:1
roaring values:9000000000000000100,9000000000000000101,9000000000000000102,9000000000000000110,9000000000000000111,9000000000000000112,
Array
(
    [0] => 9000000000000000100
    [1] => 9000000000000000101
    [2] => 9000000000000000102
    [3] => 9000000000000000110
    [4] => 9000000000000000111
    [5] => 9000000000000000112
)
