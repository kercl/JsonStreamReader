import itertools
import json

shorts = ['s', '0', 't', 'f', 'n', '${}', '$[]']

some_numbers = ["-23", "-3", "0", "5", "73", "-3.3", "0.23", "6546.3", "5e+1", "-3.2e-3"]
some_strings = ["Lorem", "ipsum", "dolor", "sit", "amet", "consetetur", "sadipscing", "elitr", "sed", "diam"]

def generate(seed_doc, max_depth, content_max_length, depth):
	if depth == max_depth:
		yield seed_doc.replace('$', '').replace(' ', '')
	relevant_shorts = shorts if depth < max_depth else shorts[:-2]

	if depth == 0:
		for s in shorts:
			yield s.replace('$', '')

	for i in range(1 if depth == 0 else content_max_length):
		for t in itertools.product(*([relevant_shorts] * (i + 1))):
			str_rep = "".join(t)
			p = seed_doc.find('$')
			if p >= 0:
				start_char = seed_doc[p+1]
				for doc in generate(seed_doc[:p] + start_char + str_rep + seed_doc[p+2:], max_depth, content_max_length, depth + 1):
					yield doc
			else:
				yield seed_doc
				break

def translate(max_depth, context_max_length):
	str_idx = 0
	num_idx = 0

	for short_representation in generate("$  ", max_depth, context_max_length, 0):
		result = ""
		key_counter = 0
		prefix_stack = [""]

		for c in short_representation:
			key_counter += 1
			prefix = prefix_stack[-1].format(key_counter)
			if c == 's':
				result += prefix + '"' + some_strings[str_idx] + '"'
				str_idx = (str_idx + 1) % len(some_strings)
			elif c == '0':
				result += prefix + some_numbers[num_idx]
				num_idx = (num_idx + 1) % len(some_numbers)
			elif c == 't':
				result += prefix + 'true'
			elif c == 'f':
				result += prefix + 'false'
			elif c == 'n':
				result += prefix + 'null'
			elif c == '{':
				result += prefix + '{'
				prefix_stack.append(',"' + some_strings[str_idx] + '_{}":')
				str_idx = (str_idx + 1) % len(some_strings)
				key_counter = 0
			elif c == '}':
				result += '}'
				prefix_stack.pop()
			elif c == '[':
				result += prefix + '['
				prefix_stack.append(',')
			elif c == ']':
				result += ']'
				prefix_stack.pop()

		yield result.replace("{,", "{").replace("[,", "[")

for doc in translate(2, 2):
	print(json.loads(doc))
