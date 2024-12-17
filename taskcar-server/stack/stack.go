package stack

type Item struct {
	Cur  *any
	next *Item
	prev *Item
}

type Stack struct {
	limit  uint32
	length uint32
	head   *Item
	tail   *Item
}

func (s Stack) GetLimit() uint32 {
	return s.limit
}

func (s Stack) GetLength() uint32 {
	return s.length
}

func (s Stack) GetHead() *Item {
	return s.head
}

func (s Stack) GetTail() *Item {
	return s.tail
}

/*
remove an element from the tail to the head and return it
*/
func (s *Stack) Pull() *any {

	if s.length == 0 {
		return nil
	}

	if s.tail == nil {
		val := s.head
		s.head = nil
		s.length--
		return val.Cur
	}

	val := s.tail
	val.next = nil

	s.tail.next.next = nil
	s.tail = s.tail.next

	s.length--
	return val.Cur

}

/*
remove an element from the head to the tail and return it
*/
func (s *Stack) Pop() *any {

	if s.length == 0 {
		return nil
	}

	if s.head == nil {
		val := s.tail
		s.tail = nil
		s.length--
		return val.Cur
	}

	val := s.head

	if s.head.prev != nil {
		val.prev.next = nil

	}

	s.head = s.head.prev

	s.length--

	return val.Cur
}

/*
Put an element from the head to the tail
*/
func (s *Stack) Push(item *any) {

	for s.limit <= s.length {
		s.Pull()
	}

	wrapped := Item{
		next: nil,
		prev: nil,
		Cur:  item,
	}

	if s.tail == nil {
		s.tail = &wrapped
		s.length++
		return
	}

	if s.head == nil {
		wrapped.prev = s.tail
		s.head = &wrapped
		s.length++
		return
	}

	temp := s.head
	temp.next = &wrapped
	wrapped.prev = temp
	s.head = &wrapped

	s.length++
}

func New(limit uint32) Stack {
	return Stack{
		limit:  limit,
		length: 0,
		tail:   nil,
		head:   nil,
	}
}
