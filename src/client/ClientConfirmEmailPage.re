module Layout = ClientLayout;

module ConfirmEmail = {
  let confirmEmail = (setError, setMsg, ~token) => {
    let%Async response = ClientApi.User.ConfirmEmail.f(~token);
    switch (response) {
    | Ok(_) =>
      setMsg(_ => Some("Thanks for confirming your email address"));
      ReasonReactRouter.push("/app/login");
    | Error(msg) =>
      setError(_ => Some("Failed to confirm email address msg=" ++ msg))
    };
    Async.async();
  };

  let parseToken = search => {
    let tokenRe = Js.Re.fromString("token\=(.*)");
    Js.Re.exec_(tokenRe, search)
    ->Belt.Option.map(Js.Re.captures)
    ->Belt.Option.flatMap(a => Belt.Array.get(a, 1))
    ->Belt.Option.flatMap(Js.Nullable.toOption);
  };

  [@react.component]
  let make = (~token) => {
    let (_, setError) =
      React.useContext(ClientContextProvider.Error.context);
    let (_, setMsg) =
      React.useContext(ClientContextProvider.Message.context);

    React.useEffect1(
      _ => {
        confirmEmail(setError, setMsg, ~token)->ignore;
        None;
      },
      [|token|],
    );

    <div>
      <h2 className="title is-2"> {React.string("Email Confirmation")} </h2>
    </div>;
  };
};

[@react.component]
let make = () => {
  let search = ReasonReactRouter.useUrl().search;
  <Layout>
    <div className="columns">
      <div className="column is-one-quarter" />
      <div className="column is-two-quarters">
        {switch (ConfirmEmail.parseToken(search)) {
         | Some(token) => <ConfirmEmail token />
         | None => ReasonReact.null
         }}
      </div>
      <div className="column is-one-quarter" />
    </div>
  </Layout>;
};
