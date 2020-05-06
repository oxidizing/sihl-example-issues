type t = {
  token: string,
  userId: string,
};

exception SessionFetchException;

let store = (~token, ~userId) => {
  open Dom.Storage;
  setItem("/users/token", token, localStorage);
  setItem("/users/userId", userId, localStorage);
};

let getOrThrow = () => {
  Dom.Storage.(
    switch (
      getItem("/users/token", localStorage),
      getItem("/users/userId", localStorage),
    ) {
    | (Some(token), Some(userId)) => {token, userId}
    | _ =>
      Js.log("ClientSession.getOrThrow() was called without session stored");
      raise(SessionFetchException);
    }
  );
};

let end_ = () => {
  open Dom.Storage;
  removeItem("/users/token", localStorage);
  removeItem("/users/userId", localStorage);
};

let has = () => {
  Dom.Storage.(
    switch (
      getItem("/users/token", localStorage),
      getItem("/users/userId", localStorage),
    ) {
    | (Some(_), Some(_)) => true
    | _ => false
    }
  );
};
