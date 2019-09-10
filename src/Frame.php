<?php

declare(strict_types=1);

namespace Spiral\Goridge;

final class Frame
{
    const HEADER_SIZE = 17;
    
    const
        PAYLOAD_NONE    = 2,
        PAYLOAD_RAW     = 4,
        PAYLOAD_ERROR   = 8,
        PAYLOAD_CONTROL = 16
    ;

    /**
     * @var string
     */
    public $body;

    /**
     * @var int
     */
    public $flags;
    
    /**
     * @param string $body
     * @param int    $flags
     */
    public function __construct(string $body = '', int $flags = 0)
    {
        $this->body  = $body;
        $this->flags = $flags;
    }

    /**
     * @return bool
     */
    public function isRaw(): bool
    {
        return ($this->flags & self::PAYLOAD_RAW) !== 0;
    }
    
    /**
     * @return bool
     */
    public function isError(): bool
    {
        return ($this->flags & self::PAYLOAD_ERROR) !== 0;
    }
    
    /**
     * @return bool
     */
    public function isControl(): bool
    {
        return ($this->flags & self::PAYLOAD_CONTROL) !== 0;
    }

    /**
     * @return string
     */
    public function __toString()
    {
        return pack($this->body, $this->flags);
    }
}
