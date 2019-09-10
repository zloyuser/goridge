<?php

declare(strict_types=1);

namespace Spiral\Goridge;

final class Relay
{
    const BUFFER_SIZE = 65535;

    /**
     * @var resource
     */
    private $input;

    /**
     * @var resource
     */
    private $output;

    /**
     * @param resource $input
     * @param resource $output
     */
    public function __construct($input, $output)
    {
        $this->guard($input, 'r');
        $this->guard($output, 'w');

        $this->input  = $input;
        $this->output = $output;
    }

    /**
     * @return self
     */
    public static function pipes(): self
    {
        return new self(\STDIN, \STDOUT);
    }

    /**
     * @param string $uri
     * @param array  $options
     *
     * @return self
     */
    public static function connect(string $uri, array $options = []): self
    {
        $flags   = \STREAM_CLIENT_CONNECT;
        $context = \stream_context_create([
            'socket' => $options,
        ]);

        $scheme = \parse_url($uri, \PHP_URL_SCHEME);

        if ($scheme && !\in_array($scheme, \stream_get_transports(), true)) {
            throw Exception\StreamException::unsupportedTransport($scheme);
        }

        if (!$client = \stream_socket_client($uri, $code, $error, 0, $flags, $context)) {
            throw Exception\StreamException::connectFailed($uri, $error, $code);
        }

        return new self($client, $client);
    }

    /**
     * @param string $data
     *
     * @return void
     */
    public function send(string $data): void
    {
        if (false === \fwrite($this->output, $data)) {
            throw Exception\StreamException::writeFailed();
        }
    }

    /**
     * @return Frame
     */
    public function receive(): Frame
    {
        $header = \fread($this->input, Frame::HEADER_SIZE);

        if (false === $header) {
            throw Exception\StreamException::readFailed();
        }

        $result  = \unpack('Cflags/Psize/Jrevs', $header);
        $payload = '';

        if (!\is_array($result)) {
            throw Exception\FrameException::invalidPrefix();
        }

        if ($result['size'] !== $result['revs']) {
            throw Exception\FrameException::invalidChecksum();
        }

        if ($result['size'] !== 0) {
            $toRead  = $result['size'];

            while ($toRead > 0) {
                $chunk = \fread($this->input, \min(self::BUFFER_SIZE, $toRead));

                if (false === $chunk) {
                    throw Exception\StreamException::readFailed();
                }

                $payload .= $chunk;
                $toRead  -= \strlen($chunk);
            }
        }

        return new Frame($payload, $result['flags']);
    }

    /**
     * @param mixed  $stream
     * @param string $mode
     *
     * @return void
     */
    private function guard($stream, string $mode): void
    {
        if (!\is_resource($stream) || \get_resource_type($stream) !== 'stream') {
            throw Exception\StreamException::unsupportedResource($stream);
        }

        $meta = \stream_get_meta_data($stream);

        if (false !== \strpos($meta['mode'], '+')) {
            return;
        }

        if (false === \strpos($meta['mode'], $mode)) {
            throw Exception\StreamException::unsupportedMode($mode);
        }
    }
}
