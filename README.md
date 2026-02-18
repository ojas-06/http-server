[HTTP](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol) is the protocol that powers the web.

<b>HTTP requests and endpoints supported in this project</b> -
<br><br>

<b>1) GET</b>
<br>
<b>&nbsp;&nbsp;Endpoints</b>
<br>

&nbsp;&nbsp;a) <b>GET /</b> (empty path) &nbsp;→&nbsp; <b>200 OK</b>
<br>

&nbsp;&nbsp;b) <b>GET /echo/{string}</b> &nbsp;→&nbsp; Responds with <code>{string}</code> in the response body
<br>
&nbsp;&nbsp;&nbsp;&nbsp;Also supports <b>gzip</b> compression when request header <code>Accept-Encoding</code> contains <code>gzip</code>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;Response includes header <code>Content-Encoding: gzip</code>
<br>

&nbsp;&nbsp;c) <b>GET /user-agent</b> &nbsp;→&nbsp; Returns the client’s
<a href="https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/User-Agent">User-Agent</a>
in the response body
<br>

&nbsp;&nbsp;d) <b>GET /files/{file-name}</b> &nbsp;→&nbsp; Returns file contents if the file exists
<br>
&nbsp;&nbsp;&nbsp;&nbsp;Else returns <b>404 Not Found</b>
<br><br>

<details>
  <summary><b>Images</b></summary>
  <br>
  <img src="terminal1.png" width="800" />
  <br><br>
  <img src="terminal2.png" width="800" />
</details>

<br><br>

<b>2) POST</b>
<br>
<b>&nbsp;&nbsp;Endpoint</b>
<br>

&nbsp;&nbsp;<b>POST /files/{file-name}</b> &nbsp;→&nbsp; Stores the request body into <code>{file-name}</code>
<br>
&nbsp;&nbsp;Returns <b>201 Created</b>
<br><br>

Also supports multiple concurrent and persistent connections.

<br><br>

<b>Performance / Load Testing</b>
<br>
Load-tested using <a href="https://github.com/wg/wrk">wrk</a> on a local VM (HTTP/1.1, <code>-t4 -c100 -d20s --latency</code>)
<br><br>

&nbsp;&nbsp;• <b>GET /</b> &nbsp;→&nbsp; ~<b>270K req/s</b>, ~<b>0.29 ms</b> median latency
<br>
&nbsp;&nbsp;• <b>GET /health</b> &nbsp;→&nbsp; ~<b>260–274K req/s</b>, ~<b>0.24–0.27 ms</b> median latency
<br>
&nbsp;&nbsp;• <b>GET /echo/abc</b> &nbsp;→&nbsp; ~<b>255–260K req/s</b>, ~<b>0.25–0.30 ms</b> median latency
<br><br>

&nbsp;&nbsp;<i>Note:</i> At peak load the server becomes CPU-bound; tail latency/timeouts may vary under VM scheduling.
