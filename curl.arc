(require "ffi.arc")
(require "json.arc")
(require "url.arc")

(w/ffi "curl"
 (cdef curl-open           "curl_easy_init"         cptr    nil)
 (cdef curl-close          "curl_easy_cleanup"      cvoid   (cptr))
 (cdef _curl_init_chunk    "arc_curl_init_chunk"    cptr    nil)
 (cdef _curl_cleanup_chunk "arc_curl_cleanup_chunk" cvoid   (cptr))
 (cdef _curl-get           "arc_curl_get"           cstring (cptr cstring cptr cint))
 (cdef _curl-post          "arc_curl_post"          cstring (cptr cstring cstring cptr cint))
 (cdef _curl-put           "arc_curl_put"           cstring (cptr cstring cstring cptr cint))
 (cdef _curl-del           "arc_curl_del"           cstring (cptr cstring cptr cint)))

(= curl-verbose* 0)
(implicit curl*)

(mac w/curl body
  `(let opend curl*
     (w/curl* (or opend (curl-open))
       (after (do ,@body)
              (unless opend
                (curl-close curl*))))))

(def curl (meth url (o data ""))
  (w/curl
    (withs (c (_curl_init_chunk)
            d (if (isa data 'string) data (build-query data))
            r (case meth
                get  (_curl-get  curl* url   c curl-verbose*)
                post (_curl-post curl* url d c curl-verbose*)
                put  (_curl-put  curl* url d c curl-verbose*)
                del  (_curl-del  curl* url   c curl-verbose*)
                     (err "Unknown method" meth)))
      (_curl_cleanup_chunk c)
      r)))

(def curl-json (meth url (o data))
  (fromjson:curl meth url (if (isa data 'string) data (tojson data))))
