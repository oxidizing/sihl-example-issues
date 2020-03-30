module Async = Sihl.Core.Async;
module Layout = ClientLayout;

module RequestPasswordReset = {
  let requestPasswordReset = (setMsg, ~email) => {
    let email = email->Belt.Option.getWithDefault("");
    let%Async _ = ClientApi.User.RequestPasswordReset.f(~email);
    setMsg(_ =>
      Some(
        "Email with instructions to reset your password sent to: " ++ email,
      )
    );
    Async.async();
  };

  [@react.component]
  let make = () => {
    let (email, setEmail) = React.useState(() => None);
    let canSubmit = Belt.Option.isSome(email);
    let (_, setMsg) =
      React.useContext(ClientContextProvider.Message.context);
    let requestPasswordReset = requestPasswordReset(setMsg);

    <div>
      <h2 className="title is-2"> {React.string("Password Reset")} </h2>
      <div className="field">
        <label className="label"> {React.string("Email address")} </label>
        <div className="control has-icons-left">
          <input
            onChange={event => {
              let email = ClientUtils.wrapFormValue(event);
              setEmail(_ => email);
            }}
            value={email->Belt.Option.getWithDefault("")}
            onKeyDown={event =>
              ReactEvent.Keyboard.which(event) === 13
                ? requestPasswordReset(~email)->ignore : ()
            }
            className="input"
            name="email"
            type_="email"
            placeholder=""
          />
          <span className="icon is-small is-left">
            <i className="fas fa-envelope" />
          </span>
        </div>
      </div>
      <div className="field is-grouped">
        <div className="control">
          <button
            className="button is-link"
            disabled={!canSubmit}
            onClick={_ => {
              let _ = requestPasswordReset(~email);
              ();
            }}>
            {React.string("Request email")}
          </button>
        </div>
      </div>
    </div>;
  };
};

module ResetPassword = {
  let resetPassword = (setError, setMsg, ~token, ~newPassword) => {
    let newPassword = newPassword->Belt.Option.getWithDefault("");
    let%Async response = ClientApi.User.ResetPassword.f(~token, ~newPassword);
    switch (response) {
    | Ok(_) =>
      setMsg(_ =>
        Some(
          "Password reset successfully, you can log in with your new password",
        )
      );
      ReasonReactRouter.push("/app/login");
    | Error(msg) =>
      setError(_ => Some("Failed to reset password msg=" ++ msg))
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

  let canSubmit = (~newPassword, ~newPasswordConfirmation) =>
    newPassword === newPasswordConfirmation;

  [@react.component]
  let make = (~token) => {
    let (newPassword, setNewPassword) = React.useState(() => None);
    let (newPasswordConfirmation, setNewPasswordConfirmation) =
      React.useState(() => None);
    let (_, setError) =
      React.useContext(ClientContextProvider.Error.context);
    let (_, setMsg) =
      React.useContext(ClientContextProvider.Message.context);
    let resetPassword = resetPassword(setError, setMsg);

    <div>
      <h2 className="title is-2"> {React.string("Password Reset")} </h2>
      <div className="field">
        <label className="label"> {React.string("New password")} </label>
        <div className="control has-icons-left">
          <input
            onChange={event => {
              let newPassword = ClientUtils.wrapFormValue(event);
              setNewPassword(_ => newPassword);
            }}
            className="input"
            name="password"
            type_="password"
            placeholder=""
          />
          <span className="icon is-small is-left">
            <i className="fas fa-lock" />
          </span>
        </div>
      </div>
      <div className="field">
        <label className="label"> {React.string("Confirm password")} </label>
        <div className="control has-icons-left">
          <input
            onChange={event => {
              let newPasswordConfirmation = ClientUtils.wrapFormValue(event);
              setNewPasswordConfirmation(_ => newPasswordConfirmation);
            }}
            value={newPasswordConfirmation->Belt.Option.getWithDefault("")}
            onKeyDown={event =>
              ReactEvent.Keyboard.which(event) === 13
                ? resetPassword(~token, ~newPassword)->ignore : ()
            }
            className="input"
            name="password"
            type_="password"
            placeholder=""
          />
          <span className="icon is-small is-left">
            <i className="fas fa-lock" />
          </span>
        </div>
      </div>
      <div className="field is-grouped">
        <div className="control">
          <button
            className="button is-link"
            disabled={!canSubmit(~newPassword, ~newPasswordConfirmation)}
            onClick={_ => {
              let _ = resetPassword(~token, ~newPassword);
              ();
            }}>
            {React.string("Set password")}
          </button>
        </div>
      </div>
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
        {switch (ResetPassword.parseToken(search)) {
         | Some(token) => <ResetPassword token />
         | None => <RequestPasswordReset />
         }}
      </div>
      <div className="column is-one-quarter" />
    </div>
  </Layout>;
};
