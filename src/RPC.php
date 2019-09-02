<?php

declare(strict_types=1);

namespace Spiral\Goridge;

final class RPC
{
    /**
     * @var Relay
     */
    private $relay;

    /**
     * @var int
     */
    private $seq;

    /**
     * @param Relay $relay
     */
    public function __construct(Relay $relay)
    {
        $this->relay = $relay;
    }

    /**
     * @param string $uri
     * @param array  $options
     *
     * @return self
     */
    public static function connect(string $uri, array $options = []): self
    {
        return new self(Relay::connect($uri, $options));
    }

    /**
     * @param string $method
     * @param mixed  $payload
     * @param bool   $raw
     *
     * @return mixed
     */
    public function call(string $method, $payload, bool $raw = false)
    {
        $call = goridge_pack($method . \pack("P", $this->seq), Frame::PAYLOAD_CONTROL | Frame::PAYLOAD_RAW);

        if ($raw) {
            $data = goridge_pack($payload, Frame::PAYLOAD_RAW);
        } else {
            $body = \json_encode($payload);

            if (\JSON_ERROR_NONE !== \json_last_error()) {
                throw Exception\JSONException::last();
            }

            $data = goridge_pack($body);
        }

        $this->relay->send($call . $data);

        $control = $this->relay->receive();

        if (!$control->isControl()) {
            throw Exception\RPCException::noControl();
        }

        $rpc = \unpack("Ps", \substr($control->body, -8));
        $rpc['m'] = \substr($control->body, 0, -8);

        if ($rpc["m"] !== $method || $rpc['s'] != $this->seq) {
            throw Exception\RPCException::controlMismatch();
        }

        $this->seq++;

        $response = $this->relay->receive();

        if ($response->isError()) {
            throw Exception\ServiceException::forResponse($response);
        }

        return $response->isRaw() ? $response->body : \json_decode($response->body, true);
    }
}
