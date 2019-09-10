<?php

namespace Spiral\Goridge {
    function pack(string $body = '', int $flags = 0) {
        $size = \strlen($body);
    
        return \pack('CPJ', $flags, $size, $size) . $body;
    }
}
