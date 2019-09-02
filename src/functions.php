<?php

function goridge_pack(string $body = '', int $flags = 0) {
    $size = \strlen($body);

    return \pack('CPJ', $flags, $size, $size) . $body;
}
