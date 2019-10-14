.section .data
.global ramdisk_start, ramdisk_end
ramdisk_start:
.incbin "fs.img"
ramdisk_end:
