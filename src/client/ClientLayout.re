module Async = Sihl.Core.Async;

module LoginRegister = {
  [@react.component]
  let make = () => {
    <div className="is-pulled-right">
      <div className="field is-grouped">
        <div className="control">
          <button
            className="button"
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              ReasonReactRouter.push("/app/login");
            }}>
            {React.string("Login")}
          </button>
        </div>
        <div className="control">
          <button
            className="button"
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              ReasonReactRouter.push("/app/register");
            }}>
            {React.string("Register")}
          </button>
        </div>
      </div>
    </div>;
  };
};

module Logout = {
  [@react.component]
  let make = () => {
    <button
      className="button is-danger is-pulled-right"
      onClick={event => {
        let _ = ReactEvent.Mouse.preventDefault(event);
        ReasonReactRouter.push("/app/login");
        ClientSession.end_();
      }}>
      {React.string("Logout")}
    </button>;
  };
};

module ForwardIfLoggedIn = {
  [@react.component]
  let make = (~children, ~url) => {
    React.useEffect1(
      _ => {
        {
          let%Async result = ClientApi.User.Me.f();
          Async.async(
            switch (result) {
            | Ok(_) => ReasonReactRouter.push(url)
            | Error(_) => ClientSession.end_()
            },
          );
        }
        ->ignore;
        None;
      },
      [|ClientSession.get().token|],
    );

    children;
  };
};

[@react.component]
let make = (~children) => {
  let (hasValidSession, setHasValidSession) =
    React.useState(_ => ClientSession.has());

  React.useEffect1(
    _ => {
      {
        let%Async result = ClientApi.User.Me.f();
        Async.async(
          switch (result) {
          | Ok(_) => setHasValidSession(_ => true)
          | Error(_) =>
            ClientSession.end_();
            setHasValidSession(_ => false);
          },
        );
      }
      ->ignore;
      None;
    },
    [|ClientSession.get().token|],
  );

  <div>
    <section className="hero is-small is-primary is-bold">
      <div className="hero-body">
        <div className="columns">
          <div className="column is-three-quarter">
            <div className="container">
              <h1 className="title">
                {React.string("Issue Management App")}
              </h1>
              <h2 className="subtitle"> {React.string("Sihl Demo")} </h2>
            </div>
          </div>
          <div className="column is-one-quarter">
            {hasValidSession ? <Logout /> : <LoginRegister />}
          </div>
        </div>
      </div>
    </section>
    <section
      className="section"
      style={ReactDOMRe.Style.make(~minHeight="40em", ())}>
      children
    </section>
    <footer className="footer">
      <div className="content has-text-centered">
        <p>
          {React.string({js|\u00a9|js})}
          <a href="https://www.oxidizing.io">
            {React.string(" Oxidizing Systems")}
          </a>
          {React.string(" | ")}
          {React.string("v0.0.1")}
        </p>
      </div>
    </footer>
  </div>;
};
