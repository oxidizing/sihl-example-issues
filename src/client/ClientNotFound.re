module Layout = ClientLayout;

[@react.component]
let make = () => {
  <Layout>
    <div className="columns">
      <div className="column is-one-quarter" />
      <div className="column is-two-quarters">
        <h2 className="title is-2"> {React.string("Not found")} </h2>
        <span>
          {React.string("Go back to the ")}
          <a
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              ReasonReactRouter.push("/app/boards");
            }}>
            {React.string("boards page")}
          </a>
        </span>
      </div>
      <div className="column is-one-quarter" />
    </div>
  </Layout>;
};
