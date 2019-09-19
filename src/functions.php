<?php

namespace Spiral\Goridge {
    if (!function_exists(__NAMESPACE__ . '\pack')) {
        function pack(string $body = '', int $flags = 0) {
            $size = \strlen($body);
        
            return \pack('CPJ', $flags, $size, $size) . $body;
        }
    }
}
