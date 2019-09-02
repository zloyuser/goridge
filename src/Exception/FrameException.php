<?php

declare(strict_types=1);

namespace Spiral\Goridge\Exception;

final class FrameException extends GoridgeException
{
    /**
     * @return self
     */
    public static function invalidPrefix(): self
    {
        return new self("Invalid frame prefix.");
    }

    /**
     * @return self
     */
    public static function invalidChecksum(): self
    {
        return new self("Invalid frame size checksum.");
    }

    /**
     * @return self
     */
    public static function invalidPayload(): self
    {
        return new self("Unable to send payload with PAYLOAD_NONE flag.");
    }
}
