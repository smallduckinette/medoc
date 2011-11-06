open Lwt
open XHTML.M
open Eliom_services
open Eliom_parameters
open Eliom_sessions
open Eliom_predefmod.Xhtml


module ThumbProps = 
struct
  let request_name = "select pgp_sym_decrypt_bytea(thumb, 'Meow') from File where fileid = $1"
end

module FileProps = 
struct
  let request_name = "select pgp_sym_decrypt_bytea(file, 'Meow') from File where fileid = $1"
end

module Input =
  Types.Singleton
    (Types.Int)

module Output = 
  Types.Singleton
    (Types.String)

module ThumbRequest = 
  Request.Request
    (Input)
    (Output)
    (ThumbProps)

module FileRequest = 
  Request.Request
    (Input)
    (Output)
    (FileProps)
    
let get_thumb dbPool fileid = 
  Lwt_preemptive.detach 
    (DbPool.run dbPool)
    (fun dbConn -> Postgresql.unescape_bytea (ThumbRequest.single_request dbConn prerr_endline fileid))

let get_file dbPool fileid = 
  Lwt_preemptive.detach 
    (DbPool.run dbPool) 
    (fun dbConn -> Postgresql.unescape_bytea (FileRequest.single_request dbConn prerr_endline fileid))

      
let medoc_thumb = Eliom_services.new_service ["thumb"] (int "fileid") ()
let medoc_file = Eliom_services.new_service ["file"] (int "fileid") ()

let _ = 
  Eliom_predefmod.Streamlist.register medoc_thumb
  (fun sp fileid () -> get_thumb Common.dbPool fileid >>= fun image -> return ([fun () -> return (Ocsigen_stream.of_string image)], "image/png"))

let _ = 
  Eliom_predefmod.Streamlist.register medoc_file
  (fun sp fileid () -> get_file Common.dbPool fileid >>= fun image -> return ([fun () -> return (Ocsigen_stream.of_string image)], "image/png"))
