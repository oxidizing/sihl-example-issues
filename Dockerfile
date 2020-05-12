# Build the ReasonReact app in a Node container
FROM node:10.16.3 AS js-builder
WORKDIR /home/sihl/app
COPY package.json yarn.lock bsconfig.json ./
COPY src src
COPY static static
RUN yarn
RUN yarn build

# Build the Sihl backend in a OPAM/OCaml container
FROM ocaml/opam2:4.08 AS ocaml-builder
WORKDIR app
COPY sihl_example_issues.opam .
COPY dune-project .

RUN opam pin add -yn sihl_core https://github.com/oxidizing/sihl.git\#0.0.27 && \
        opam pin add -yn sihl_email https://github.com/oxidizing/sihl.git\#0.0.27 && \
        opam pin add -yn sihl_user https://github.com/oxidizing/sihl.git\#0.0.27 && \
        opam pin add -yn sihl_example_issues . && \
        opam depext -y sihl_example_issues && \
        opam install --deps-only sihl_example_issues

COPY Makefile .
COPY src src
RUN sudo chown -R opam:nogroup . && \
        opam config exec -- make

# Copy over the binaries from stage 1 and 2 and install systems dependencies
FROM debian:10-slim
WORKDIR /app
# TODO use output of previous opam depext to automatically fetch correct system deps
RUN apt-get update -y && \
        apt-get install -qq -yy emacs-nox libffi-dev libgmp-dev libpcre3-dev libpq-dev libssl-dev m4 perl pkg-config
# WTF: https://github.com/mirage/ocaml-cohttp/issues/675
RUN echo "http		80/tcp		www		# WorldWideWeb HTTP" >> /etc/services
RUN echo "https          443/tcp	www		# WorldWideWeb HTTPS" >> /etc/services
COPY --from=ocaml-builder /home/opam/opam-repository/app/_build/default/src/bin/Run.exe run.exe
COPY --from=js-builder /home/sihl/app/dist static
ENV SIHL_ENV production
CMD ["/app/run.exe", "start"]
