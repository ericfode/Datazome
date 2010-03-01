package genome

type Gene []byte

func (gene Gene) GetData() []byte { return gene }
