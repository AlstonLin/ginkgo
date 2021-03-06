#include <ginkgo/gtk-widgets/TextBox.h>

namespace ginkgo
{
namespace gtkWidgets
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TextBox
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TextBox::TextBox()
{
	add(m_listBox);
	m_listBox.set_selection_mode(Gtk::SELECTION_MULTIPLE);
	show_all();

	m_listBox.signal_selected_rows_changed().connect(m_onSelectedRowsChanged);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TextBox::setTitle(std::string title)
{
	//m_title.set_markup("<b>" + title + "</b>");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TextBox::addItem(std::string itemText)
{
	auto label = std::make_unique<Gtk::Label>();
	label->set_markup(itemText);
	label->show();

	m_listBox.add(*label);

	m_labels.push_back(std::move(label));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TextBox::clear()
{
	m_labels.clear();

	auto children = m_listBox.get_children();

	std::for_each(m_labels.begin(), m_labels.end(),
		[&](const auto &child)
		{
			m_listBox.remove(*child);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TextBox::selectAll()
{
	m_listBox.select_all();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

sigc::signal<void> &TextBox::onSelectedRowsChanged()
{
	return m_onSelectedRowsChanged;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Gtk::ListBoxRow *> TextBox::selectedRows()
{
	return m_listBox.get_selected_rows();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<Gtk::ListBoxRow *> TextBox::selectedRows() const
{
	return m_listBox.get_selected_rows();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
