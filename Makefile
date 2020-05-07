build:
	@dune build @install

clean:
	@dune clean

test:
	SIHL_ENV=test dune runtest --force --no-buffer
